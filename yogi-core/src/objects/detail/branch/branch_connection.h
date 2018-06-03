#pragma once

#include "../../../config.h"
#include "../../../utils/socket.h"
#include "../../context.h"
#include "../../logger.h"
#include "branch_info.h"

#include <boost/asio.hpp>
#include <functional>
#include <memory>

namespace objects {
namespace detail {

class BranchConnection final
    : public std::enable_shared_from_this<BranchConnection> {
 public:
  typedef std::function<void(const api::Error&)> CompletionHandler;

  BranchConnection(utils::TimedTcpSocketPtr socket, BranchInfoPtr local_info);

  BranchInfoPtr GetRemoteBranchInfo() const { return remote_info_; }
  std::string MakeInfoString() const;
  const boost::asio::ip::tcp::endpoint& GetRemoteEndpoint() const {
    return socket_->GetRemoteEndpoint();
  }

  void ExchangeBranchInfo(CompletionHandler handler);
  void Authenticate(utils::SharedByteVector password_hash,
                    CompletionHandler handler);
  void RunSession(CompletionHandler handler);

 private:
  void OnInfoSent(CompletionHandler handler);
  void OnInfoHeaderReceived(const utils::ByteVector& info_msg_hdr,
                            CompletionHandler handler);
  void OnInfoBodyReceived(const utils::ByteVector& info_msg_hdr,
                          const utils::ByteVector& info_msg_body,
                          CompletionHandler handler);
  void OnChallengeSent(utils::SharedByteVector my_challenge,
                       utils::SharedByteVector password_hash,
                       CompletionHandler handler);
  void OnChallengeReceived(const utils::ByteVector& remote_challenge,
                           utils::SharedByteVector my_challenge,
                           utils::SharedByteVector password_hash,
                           CompletionHandler handler);
  utils::SharedByteVector SolveChallenge(
      const utils::ByteVector& challenge,
      const utils::ByteVector& password_hash) const;
  void OnSolutionSent(utils::SharedByteVector my_solution,
                      CompletionHandler handler);
  void OnSolutionReceived(const utils::ByteVector& received_solution,
                          utils::SharedByteVector my_solution,
                          CompletionHandler handler);

  static const LoggerPtr logger_;

  const utils::TimedTcpSocketPtr socket_;
  const BranchInfoPtr local_info_;
  const utils::Timestamp connected_since_;
  BranchInfoPtr remote_info_;
};

typedef std::shared_ptr<BranchConnection> BranchConnectionPtr;

}  // namespace detail
}  // namespace objects
