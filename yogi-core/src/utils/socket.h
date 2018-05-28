#pragma once

#include "../config.h"
#include "../objects/context.h"
#include "../api/error.h"

#include <boost/asio.hpp>
#include <memory>
#include <chrono>
#include <sstream>
#include <vector>
#include <functional>

namespace utils {

class TimedTcpSocket : public std::enable_shared_from_this<TimedTcpSocket> {
 public:
  typedef std::function<void(const api::Error&)> CompletionHandler;
  typedef std::function<void(const api::Error&, const std::vector<char>&)>
      ReceiveHandler;

  TimedTcpSocket(objects::ContextPtr context, std::chrono::nanoseconds timeout);
  virtual ~TimedTcpSocket() {}

  std::weak_ptr<TimedTcpSocket> MakeWeakPtr() { return shared_from_this(); }

  const boost::asio::ip::tcp::endpoint& GetRemoteEndpoint() const {
    return remote_ep_;
  }

  void Accept(boost::asio::ip::tcp::acceptor* acceptor,
              CompletionHandler handler);
  void Connect(const boost::asio::ip::tcp::endpoint& ep,
               CompletionHandler handler);
  void Send(const boost::asio::const_buffer& buffer, CompletionHandler handler);
  void ReceiveExactly(std::size_t num_bytes, ReceiveHandler handler);

 private:
  void StartTimeout(std::weak_ptr<TimedTcpSocket> weak_self);
  void OnTimeout();

  const std::chrono::nanoseconds timeout_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::endpoint remote_ep_;
  boost::asio::steady_timer timer_;
  bool timed_out_;
  std::shared_ptr<std::vector<char>> rcv_buffer_;
};

typedef std::shared_ptr<TimedTcpSocket> TimedTcpSocketPtr;

}  // namespace utils

std::ostream& operator<<(std::ostream& os, const utils::TimedTcpSocket& socket);