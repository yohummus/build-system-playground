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

#include "branch.h"
#include "../network/ip.h"

#include <chrono>
using namespace std::chrono_literals;

namespace objects {

Branch::Branch(ContextPtr context, std::string name, std::string description,
               std::string net_name, std::string password, std::string path,
               const std::vector<std::string>& adv_if_strings,
               const boost::asio::ip::udp::endpoint& adv_ep,
               std::chrono::nanoseconds adv_interval,
               std::chrono::nanoseconds timeout, bool ghost_mode,
               std::size_t tx_queue_size, std::size_t rx_queue_size)
    : context_(context),
      connection_manager_(std::make_shared<detail::ConnectionManager>(
          context, password, adv_if_strings, adv_ep,
          [&](auto& res, auto conn) { this->OnConnectionChanged(res, conn); },
          [&](auto& msg, auto& conn) {
            this->OnMessageReceived(msg, conn);
          })),
      info_(std::make_shared<detail::LocalBranchInfo>(
          name, description, net_name, path,
          connection_manager_->GetAdvertisingInterfaces(),
          connection_manager_->GetAdvertisingEndpoint(),
          connection_manager_->GetTcpServerEndpoint(), timeout, adv_interval,
          ghost_mode, tx_queue_size, rx_queue_size)),
      broadcast_manager_(std::make_shared<detail::BroadcastManager>(
          context, *connection_manager_)) {
  if (name.empty() || net_name.empty() || path.empty() || path.front() != '/' ||
      adv_interval < 1ms || timeout < 1ms) {
    throw api::Error(YOGI_ERR_INVALID_PARAM);
  }
}

void Branch::Start() { connection_manager_->Start(info_); }

const boost::uuids::uuid& Branch::GetUuid() const { return info_->GetUuid(); }

std::string Branch::MakeInfoString() const { return info_->ToJson().dump(); }

Branch::BranchInfoStringsList Branch::MakeConnectedBranchesInfoStrings() const {
  return connection_manager_->MakeConnectedBranchesInfoStrings();
}

void Branch::AwaitEventAsync(api::BranchEvents events,
                             BranchEventHandler handler) {
  connection_manager_->AwaitEventAsync(events, handler);
}

void Branch::CancelAwaitEvent() { connection_manager_->CancelAwaitEvent(); }

Branch::SendBroadcastOperationId Branch::SendBroadcastAsync(
    const network::UserData& user_data, bool retry,
    SendBroadcastHandler handler) {
  return broadcast_manager_->SendBroadcastAsync(user_data, retry, handler);
}

api::Result Branch::SendBroadcast(const network::UserData& user_data,
                                  bool block) {
  return broadcast_manager_->SendBroadcast(user_data, block);
}

bool Branch::CancelSendBroadcast(SendBroadcastOperationId oid) {
  return broadcast_manager_->CancelSendBroadcast(oid);
}

void Branch::ReceiveBroadcast(api::Encoding enc,
                              boost::asio::mutable_buffer data,
                              ReceiveBroadcastHandler handler) {
  broadcast_manager_->ReceiveBroadcast(enc, data, handler);
}

bool Branch::CancelReceiveBroadcast() {
  return broadcast_manager_->CancelReceiveBroadcast();
}

void Branch::OnConnectionChanged(const api::Result& res,
                                 const detail::BranchConnectionPtr& conn) {
  YOGI_LOG_INFO(logger_, "Connection to " << conn->GetRemoteBranchInfo()
                                          << " changed: " << res);
  // TODO
}

void Branch::OnMessageReceived(const network::IncomingMessage& msg,
                               const detail::BranchConnectionPtr& conn) {
  YOGI_LOG_FATAL(logger_, "Msg received: " << msg);
}

const LoggerPtr Branch::logger_ = Logger::CreateStaticInternalLogger("Branch");

}  // namespace objects
