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
#include "../utils/ip.h"

#include <chrono>
using namespace std::chrono_literals;

namespace objects {

Branch::Branch(ContextPtr context, std::string name, std::string description,
               std::string net_name, std::string password, std::string path,
               const boost::asio::ip::udp::endpoint& adv_ep,
               std::chrono::nanoseconds adv_interval,
               std::chrono::nanoseconds timeout, bool ghost_mode)
    : context_(context),
      connection_manager_(std::make_shared<detail::ConnectionManager>(
          context, password, adv_ep,
          [&](auto& err, auto conn) { this->OnConnectionChanged(err, conn); },
          [&](auto& msg, auto size, auto& conn) {
            this->OnMessageReceived(msg, size, conn);
          })),
      info_(detail::BranchInfo::CreateLocal(
          name, description, net_name, path,
          connection_manager_->GetTcpServerEndpoint(), timeout, adv_interval,
          ghost_mode)),
      broadcast_manager_(std::make_shared<detail::BroadcastManager>(context)) {
  if (name.empty() || net_name.empty() || path.empty() || path.front() != '/' ||
      adv_interval < 1ms || timeout < 1ms) {
    throw api::Error(YOGI_ERR_INVALID_PARAM);
  }
}

void Branch::Start() { connection_manager_->Start(info_); }

std::string Branch::MakeInfoString() const {
  auto json = info_->ToJson();
  auto& ep = connection_manager_->GetAdvertisingEndpoint();
  json["advertising_address"] = utils::MakeIpAddressString(ep);
  json["advertising_port"] = ep.port();
  return json.dump();
}

Branch::BranchInfoStringsList Branch::MakeConnectedBranchesInfoStrings() const {
  return connection_manager_->MakeConnectedBranchesInfoStrings();
}

void Branch::OnConnectionChanged(const api::Error& err,
                                 const detail::BranchConnectionPtr& conn) {
  YOGI_LOG_INFO(logger_, "Connection to " << conn->GetRemoteBranchInfo()
                                          << " changed: " << err);
  // TODO
}

void Branch::OnMessageReceived(const utils::ByteVector& msg, std::size_t size,
                               const detail::BranchConnectionPtr& conn) {
  // TODO
}

const LoggerPtr Branch::logger_ = Logger::CreateStaticInternalLogger("Branch");

}  // namespace objects
