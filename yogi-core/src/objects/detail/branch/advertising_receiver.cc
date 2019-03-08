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
  StartReceiveAdvertisement();
}

void AdvertisingReceiver::SetupSocket() {
  using namespace boost::asio::ip;

  boost::system::error_code ec;
  socket_.open(adv_ep_.protocol(), ec);
  if (ec) throw api::Error(YOGI_ERR_OPEN_SOCKET_FAILED);

  socket_.set_option(udp::socket::reuse_address(true), ec);
  if (ec) throw api::Error(YOGI_ERR_SET_SOCKET_OPTION_FAILED);

  address listen_addr;
  if (adv_ep_.address().is_v6()) {
    listen_addr = address(address_v6::any());
  } else {
    listen_addr = address(address_v6::any());
  }

  socket_.bind(udp::endpoint(listen_addr, adv_ep_.port()), ec);
  if (ec) throw api::Error(YOGI_ERR_BIND_SOCKET_FAILED);

  JoinMulticastGroup();
}

void AdvertisingReceiver::JoinMulticastGroup() {
  using namespace boost::asio::ip;

  boost::system::error_code ec;
  socket_.set_option(multicast::join_group(adv_ep_.address()), ec);

  if (ec && adv_ep_.address().is_v6()) {
    unsigned long net_if = 0;
    do {
      socket_.set_option(
          multicast::join_group(adv_ep_.address().to_v6(), net_if), ec);
      ++net_if;
    } while (ec && net_if < 20);
  }

  if (ec) throw api::Error(YOGI_ERR_JOIN_MULTICAST_GROUP_FAILED);
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
