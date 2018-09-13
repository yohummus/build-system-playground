/*
 * This file is part of the Yogi distribution https://github.com/yohummus/yogi.
 * Copyright (c) 2018 Johannes Bergmann.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../../config.h"
#include "../../../api/enums.h"
#include "advertising_receiver.h"
#include "advertising_sender.h"
#include "branch_connection.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/functional/hash.hpp>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <mutex>

namespace objects {
namespace detail {

class ConnectionManager final
    : public std::enable_shared_from_this<ConnectionManager> {
 public:
  using ByteVector = utils::ByteVector;
  typedef std::function<void(const api::Error&, api::BranchEvents,
                             const api::Error&, const boost::uuids::uuid&,
                             const std::string&)>
      BranchEventHandler;
  typedef std::function<void(const api::Error&, BranchConnectionPtr)>
      ConnectionChangedHandler;
  typedef std::function<void(const ByteVector&, std::size_t,
                             const BranchConnectionPtr&)>
      MessageHandler;
  typedef std::vector<std::pair<boost::uuids::uuid, std::string>>
      BranchInfoStringsList;

  ConnectionManager(ContextPtr context, const std::string& password,
                    const boost::asio::ip::udp::endpoint& adv_ep,
                    ConnectionChangedHandler connection_changed_handler,
                    MessageHandler message_handler);
  virtual ~ConnectionManager();

  void Start(BranchInfoPtr info);

  const boost::asio::ip::udp::endpoint& GetAdvertisingEndpoint() const {
    return adv_sender_->GetEndpoint();
  }

  boost::asio::ip::tcp::endpoint GetTcpServerEndpoint() const {
    return acceptor_.local_endpoint();
  }

  BranchInfoStringsList MakeConnectedBranchesInfoStrings() const;

  void AwaitEvent(api::BranchEvents events, BranchEventHandler handler);
  void CancelAwaitEvent();

 private:
  typedef std::unordered_set<boost::uuids::uuid,
                             boost::hash<boost::uuids::uuid>>
      UuidSet;

  typedef std::unordered_map<boost::uuids::uuid, detail::BranchConnectionPtr,
                             boost::hash<boost::uuids::uuid>>
      ConnectionsMap;
  typedef ConnectionsMap::value_type ConnectionsMapEntry;
  typedef std::set<utils::TimedTcpSocketPtr> SocketSet;
  typedef std::set<BranchConnectionPtr> ConnectionSet;

  void SetupAcceptor(const boost::asio::ip::tcp& protocol);
  void StartAccept();
  void OnAcceptFinished(const api::Error& err, utils::TimedTcpSocketPtr socket);
  void OnAdvertisementReceived(const boost::uuids::uuid& adv_uuid,
                               const boost::asio::ip::tcp::endpoint& ep);
  void OnConnectFinished(const api::Error& err,
                         const boost::uuids::uuid& adv_uuid,
                         utils::TimedTcpSocketPtr socket);
  void StartExchangeBranchInfo(utils::TimedTcpSocketPtr socket,
                               const boost::uuids::uuid& adv_uuid);
  void OnExchangeBranchInfoFinished(const api::Error& err,
                                    BranchConnectionPtr conn,
                                    const boost::uuids::uuid& adv_uuid);
  bool CheckExchangeBranchInfoError(const api::Error& err,
                                    const BranchConnectionPtr& conn);
  bool VerifyUuidsMatch(const boost::uuids::uuid& remote_uuid,
                        const boost::uuids::uuid& adv_uuid);
  bool VerifyUuidNotBlacklisted(const boost::uuids::uuid& uuid);
  bool VerifyConnectionHasHigherPriority(bool conn_already_exists,
                                         const BranchConnectionPtr& conn);
  void PublishExchangeBranchInfoError(const api::Error& err,
                                      BranchConnectionPtr conn,
                                      ConnectionsMapEntry* entry);
  api::Error CheckRemoteBranchInfo(const BranchInfoPtr& remote_info);
  void StartAuthenticate(BranchConnectionPtr conn);
  void OnAuthenticateFinished(const api::Error& err, BranchConnectionPtr conn);
  void StartSession(BranchConnectionPtr conn);
  void OnSessionTerminated(const api::Error& err, BranchConnectionPtr conn);
  utils::TimedTcpSocketPtr MakeSocketAndKeepItAlive();
  utils::TimedTcpSocketPtr StopKeepingSocketAlive(
      const utils::TimedTcpSocketWeakPtr& weak_socket);
  BranchConnectionPtr MakeConnectionAndKeepItAlive(
      utils::TimedTcpSocketPtr socket);
  BranchConnectionPtr StopKeepingConnectionAlive(
      const BranchConnectionWeakPtr& weak_conn);

  template <typename Fn>
  void EmitBranchEvent(api::BranchEvents event, const api::Error& ev_res,
                       const boost::uuids::uuid& uuid, Fn make_json_fn);
  void EmitBranchEvent(api::BranchEvents event, const api::Error& ev_res,
                       const boost::uuids::uuid& uuid);

  template <typename Fn>
  void LogBranchEvent(api::BranchEvents event, const api::Error& ev_res,
                      Fn make_json_fn);

  static const LoggerPtr logger_;

  const ContextPtr context_;
  const utils::SharedByteVector password_hash_;
  const ConnectionChangedHandler connection_changed_handler_;
  const MessageHandler message_handler_;
  const detail::AdvertisingSenderPtr adv_sender_;
  const detail::AdvertisingReceiverPtr adv_receiver_;
  boost::asio::ip::tcp::acceptor acceptor_;
  SocketSet sockets_kept_alive_;
  ConnectionSet connections_kept_alive_;
  BranchInfoPtr info_;

  UuidSet blacklisted_uuids_;
  UuidSet pending_connects_;
  ConnectionsMap connections_;
  mutable std::mutex connections_mutex_;

  BranchEventHandler event_handler_;
  api::BranchEvents observed_events_;
  std::recursive_mutex event_mutex_;
};

typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;

}  // namespace detail
}  // namespace objects
