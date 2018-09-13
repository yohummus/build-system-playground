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

#include "../config.h"
#include "context.h"
#include "detail/branch/broadcast_manager.h"
#include "detail/branch/connection_manager.h"

namespace objects {

class Branch : public api::ExposedObjectT<Branch, api::ObjectType::kBranch> {
 public:
  using BranchEventHandler = detail::ConnectionManager::BranchEventHandler;
  using BranchInfoStringsList =
      detail::ConnectionManager::BranchInfoStringsList;

  Branch(ContextPtr context, std::string name, std::string description,
         std::string net_name, std::string password, std::string path,
         const boost::asio::ip::udp::endpoint& adv_ep,
         std::chrono::nanoseconds adv_interval,
         std::chrono::nanoseconds timeout, bool ghost_mode);

  void Start();

  const boost::uuids::uuid& GetUuid() const { return info_->GetUuid(); }
  std::string MakeInfoString() const;
  BranchInfoStringsList MakeConnectedBranchesInfoStrings() const;

  void AwaitEvent(api::BranchEvents events, BranchEventHandler handler) {
    connection_manager_->AwaitEvent(events, handler);
  }

  void CancelAwaitEvent() { connection_manager_->CancelAwaitEvent(); }

 private:
  void OnConnectionChanged(const api::Error& err,
                           const detail::BranchConnectionPtr& conn);
  void OnMessageReceived(const utils::ByteVector& msg, std::size_t size,
                         const detail::BranchConnectionPtr& conn);

  static const LoggerPtr logger_;

  const ContextPtr context_;
  const detail::ConnectionManagerPtr connection_manager_;
  const detail::BranchInfoPtr info_;
  const detail::BroadcastManagerPtr broadcast_manager_;
};

typedef std::shared_ptr<Branch> BranchPtr;

}  // namespace objects
