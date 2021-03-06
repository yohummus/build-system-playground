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
#include "../../../network/tcp_transport.h"
#include "../../../utils/system.h"
#include "advertising_receiver.h"
#include "advertising_sender.h"
#include "branch_connection.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/functional/hash.hpp>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <atomic>

namespace objects {
namespace detail {

class ConnectionManager;
typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;
typedef std::weak_ptr<ConnectionManager> ConnectionManagerWeakPtr;

class ConnectionManager
    : public std::enable_shared_from_this<ConnectionManager> {
 public:
  using ByteVector = utils::ByteVector;
  typedef std::function<void(const api::Result&, api::BranchEvents,
                             const api::Result&, const boost::uuids::uuid&,
                             const std::string&)>
      BranchEventHandler;
  typedef std::function<void(const api::Result&, BranchConnectionPtr)>
      ConnectionChangedHandler;
  typedef std::function<void(const network::IncomingMessage&,
                             const BranchConnectionPtr&)>
      MessageReceiveHandler;
  typedef std::vector<std::pair<boost::uuids::uuid, std::string>>
      BranchInfoStringsList;
  using OperationTag = network::MessageTransport::OperationTag;

  ConnectionManager(ContextPtr context, const std::string& password,
                    const std::vector<std::string>& adv_if_strings,
                    const boost::asio::ip::udp::endpoint& adv_ep,
                    ConnectionChangedHandler connection_changed_handler,
                    MessageReceiveHandler message_handler);
  virtual ~ConnectionManager();

  void Start(LocalBranchInfoPtr info);

  const std::vector<utils::NetworkInterfaceInfo>& GetAdvertisingInterfaces()
      const {
    return adv_ifs_;
  }

  const boost::asio::ip::udp::endpoint& GetAdvertisingEndpoint() const {
    return adv_sender_->GetEndpoint();
  }

  boost::asio::ip::tcp::endpoint GetTcpServerEndpoint() const {
    return acceptor_.local_endpoint();
  }

  BranchInfoStringsList MakeConnectedBranchesInfoStrings() const;

  bool AwaitEventAsync(api::BranchEvents events, BranchEventHandler handler);
  bool CancelAwaitEvent();

  template <typename Fn>
  void ForeachRunningSession(Fn fn) {
    std::lock_guard<std::mutex> lock(connections_mutex_);
    for (auto& entry : connections_) {
      auto& conn = entry.second;
      if (conn->SessionRunning()) {
        fn(conn);
      }
    }
  }

  OperationTag MakeOperationId();

 private:
  typedef std::unordered_set<boost::uuids::uuid,
                             boost::hash<boost::uuids::uuid>>
      UuidSet;

  typedef std::unordered_map<boost::uuids::uuid, detail::BranchConnectionPtr,
                             boost::hash<boost::uuids::uuid>>
      ConnectionsMap;

  typedef ConnectionsMap::value_type ConnectionsMapEntry;
  typedef std::set<network::TcpTransport::ConnectGuardPtr> ConnectGuardsSet;
  typedef std::set<BranchConnectionPtr> ConnectionsSet;

  ConnectionManagerWeakPtr MakeWeakPtr() { return {shared_from_this()}; }
  void SetupAcceptor(const boost::asio::ip::tcp& protocol);
  void StartAccept();
  void OnAcceptFinished(const api::Result& res,
                        network::TcpTransportPtr transport);
  void OnAdvertisementReceived(const boost::uuids::uuid& adv_uuid,
                               const boost::asio::ip::tcp::endpoint& ep);
  void OnConnectFinished(const api::Result& res,
                         const boost::uuids::uuid& adv_uuid,
                         network::TcpTransportPtr transport);
  void StartExchangeBranchInfo(network::TransportPtr transport,
                               const boost::asio::ip::address& peer_address,
                               const boost::uuids::uuid& adv_uuid);
  void OnExchangeBranchInfoFinished(const api::Result& res,
                                    BranchConnectionPtr conn,
                                    const boost::uuids::uuid& adv_uuid);
  bool CheckExchangeBranchInfoError(const api::Result& res,
                                    const BranchConnectionPtr& conn);
  bool VerifyUuidsMatch(const boost::uuids::uuid& remote_uuid,
                        const boost::uuids::uuid& adv_uuid);
  bool VerifyUuidNotBlacklisted(const boost::uuids::uuid& uuid);
  bool VerifyConnectionHasHigherPriority(bool conn_already_exists,
                                         const BranchConnectionPtr& conn);
  void PublishExchangeBranchInfoError(const api::Error& err,
                                      BranchConnectionPtr conn,
                                      ConnectionsMapEntry* entry);
  api::Result CheckRemoteBranchInfo(const BranchInfoPtr& remote_info);
  void StartAuthenticate(BranchConnectionPtr conn);
  void OnAuthenticateFinished(const api::Result& res, BranchConnectionPtr conn);
  void StartSession(BranchConnectionPtr conn);
  void OnSessionTerminated(const api::Error& err, BranchConnectionPtr conn);
  BranchConnectionPtr MakeConnectionAndKeepItAlive(
      const boost::asio::ip::address& peer_address,
      network::TransportPtr transport);
  BranchConnectionPtr StopKeepingConnectionAlive(
      const BranchConnectionWeakPtr& weak_conn);

  template <typename Fn>
  void EmitBranchEvent(api::BranchEvents event, const api::Result& ev_res,
                       const boost::uuids::uuid& uuid, Fn make_json_fn);
  void EmitBranchEvent(api::BranchEvents event, const api::Result& ev_res,
                       const boost::uuids::uuid& uuid);

  template <typename Fn>
  void LogBranchEvent(api::BranchEvents event, const api::Result& ev_res,
                      Fn make_json_fn);

  static const LoggerPtr logger_;

  const ContextPtr context_;
  const std::vector<utils::NetworkInterfaceInfo> adv_ifs_;
  const utils::SharedByteVector password_hash_;
  const ConnectionChangedHandler connection_changed_handler_;
  const MessageReceiveHandler message_handler_;
  const detail::AdvertisingSenderPtr adv_sender_;
  const detail::AdvertisingReceiverPtr adv_receiver_;
  boost::asio::ip::tcp::acceptor acceptor_;
  network::TcpTransport::AcceptGuardPtr accept_guard_;
  ConnectGuardsSet connect_guards_;
  ConnectionsSet connections_kept_alive_;
  LocalBranchInfoPtr info_;

  UuidSet blacklisted_uuids_;
  UuidSet pending_connects_;
  ConnectionsMap connections_;
  mutable std::mutex connections_mutex_;

  std::atomic<OperationTag> last_op_tag_;

  BranchEventHandler event_handler_;
  api::BranchEvents observed_events_;
  std::recursive_mutex event_mutex_;
};

}  // namespace detail
}  // namespace objects
