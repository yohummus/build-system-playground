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

#include "advertising_receiver.h"
#include "../../../api/errors.h"

#include <boost/asio/ip/multicast.hpp>

namespace objects {
namespace detail {

AdvertisingReceiver::AdvertisingReceiver(
    ContextPtr context, const boost::asio::ip::udp::endpoint& adv_ep,
    ObserverFn observer_fn)
    : context_(context),
      adv_ep_(adv_ep),
      observer_fn_(observer_fn),
      buffer_(
          utils::MakeSharedByteVector(BranchInfo::kAdvertisingMessageSize + 1)),
      socket_(context->IoContext()) {
  SetupSocket();
}

void AdvertisingReceiver::Start(LocalBranchInfoPtr info) {
  YOGI_ASSERT(!info_);

  info_ = info;
  if (JoinMulticastGroups()) {
    StartReceiveAdvertisement();
  }
}

void AdvertisingReceiver::SetupSocket() {
  using namespace boost::asio::ip;

  boost::system::error_code ec;
  socket_.open(adv_ep_.protocol(), ec);
  if (ec) throw api::Error(YOGI_ERR_OPEN_SOCKET_FAILED);

  socket_.set_option(udp::socket::reuse_address(true), ec);
  if (ec) throw api::Error(YOGI_ERR_SET_SOCKET_OPTION_FAILED);

  socket_.bind(udp::endpoint(adv_ep_.protocol(), adv_ep_.port()), ec);
  if (ec) throw api::Error(YOGI_ERR_BIND_SOCKET_FAILED);
}

bool AdvertisingReceiver::JoinMulticastGroups() {
  using namespace boost::asio::ip;

  bool joined_at_least_once = false;
  for (auto& ifc : info_->GetAdvertisingInterfaces()) {
    for (auto& addr : ifc.addresses) {
      boost::system::error_code ec;
      auto opt = addr.is_v6() ? multicast::join_group(adv_ep_.address().to_v6(),
                                                      addr.to_v6().scope_id())
                              : multicast::join_group(adv_ep_.address().to_v4(),
                                                      addr.to_v4());
      socket_.set_option(opt, ec);

      if (ec) {
        YOGI_LOG_ERROR(logger_,
                       info_ << " Could not join advertising multicast group "
                             << adv_ep_ << " for interface " << addr << ": "
                             << ec.message()
                             << ". This interface will be ignored.");
        continue;
      }

      YOGI_LOG_INFO(logger_, info_ << " Using interface " << addr
                                   << " for receiving advertising messages.");
      joined_at_least_once = true;
    }

    if (!joined_at_least_once) {
      YOGI_LOG_ERROR(logger_, info_ << " No network interfaces available for "
                                       "receiving advertising messages.");
    }
  }

  return joined_at_least_once;
}

void AdvertisingReceiver::StartReceiveAdvertisement() {
  auto buffer = buffer_;
  auto weak_self = std::weak_ptr<AdvertisingReceiver>{shared_from_this()};
  socket_.async_receive_from(boost::asio::buffer(*buffer_), sender_ep_,
                             [weak_self, buffer](auto ec, auto bytes_received) {
                               auto self = weak_self.lock();
                               if (!self) return;

                               self->OnReceivedAdvertisementFinished(
                                   ec, bytes_received);
                             });
}

void AdvertisingReceiver::OnReceivedAdvertisementFinished(
    const boost::system::error_code& ec, std::size_t bytes_received) {
  if (ec) {
    YOGI_LOG_ERROR(
        logger_,
        info_ << "Failed to receive advertising message: " << ec.message()
              << ". No more advertising messages will be received.");
    return;
  }

  if (bytes_received != BranchInfo::kAdvertisingMessageSize) {
    YOGI_LOG_WARNING(logger_,
                     info_ << "Unexpected advertising message size received");
    StartReceiveAdvertisement();
    return;
  }

  boost::uuids::uuid uuid;
  unsigned short tcp_port;
  auto res = RemoteBranchInfo::DeserializeAdvertisingMessage(&uuid, &tcp_port,
                                                             *buffer_);
  if (res.IsError()) {
    YOGI_LOG_WARNING(logger_,
                     info_ << " Invalid advertising message received from "
                           << sender_ep_.address() << ": " << res);
    StartReceiveAdvertisement();
    return;
  }

  // Ignore advertising messages that we sent ourself
  if (uuid != info_->GetUuid()) {
    auto tcp_ep =
        boost::asio::ip::tcp::endpoint(sender_ep_.address(), tcp_port);
    observer_fn_(uuid, tcp_ep);
  }

  StartReceiveAdvertisement();
}

const LoggerPtr AdvertisingReceiver::logger_ =
    Logger::CreateStaticInternalLogger("Branch.AdvertisingReceiver");

}  // namespace detail
}  // namespace objects
