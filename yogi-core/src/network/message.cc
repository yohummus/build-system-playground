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

#include "message.h"
#include "../api/errors.h"

#include <nlohmann/json.hpp>
#include <msgpack.hpp>

namespace network {
namespace internal {

struct MsgPackCheckVisitor : public msgpack::null_visitor {
  void parse_error(std::size_t parsed_offset, std::size_t error_offset) {
    throw api::DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK)
        << "Parse errror. Parsed offset: " << parsed_offset
        << "; Error offset: " << error_offset;
  }

  void insufficient_bytes(std::size_t parsed_offset, std::size_t error_offset) {
    throw api::DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK)
        << "Insufficient bytes. Parsed offset: " << parsed_offset
        << "; Error offset: " << error_offset;
  }
};

}  // namespace internal

Message::Message(MessageType type) : type_(type), header_size_(0) {
  if (type_ != MessageType::kHeartbeat) {
    msg_.push_back(type_);
  }
}

Message::Message(MessageType type, boost::asio::const_buffer header,
                 boost::asio::const_buffer user_data, api::Encoding user_enc)
    : type_(type), header_size_(header.size()) {
  YOGI_ASSERT(type != MessageType::kHeartbeat);
  PopulateMsg(header, user_data, user_enc);
}

Message::Message(MessageType type, boost::asio::const_buffer header)
    : Message(type, header, {}, {}) {}

Message::Message(MessageType type, boost::asio::const_buffer user_data,
                 api::Encoding user_enc)
    : Message(type, {}, user_data, user_enc) {}

std::size_t Message::GetSize() const { return GetMessageAsBytes().size(); }

utils::ByteVector Message::GetHeader() const {
  auto& msg = GetMessageAsBytes();
  return utils::ByteVector(
      msg.begin() + 1,
      msg.begin() + 1 +
          static_cast<utils::ByteVector::difference_type>(header_size_));
}

std::size_t Message::GetUserDataSize() const {
  return GetSize() - 1 - header_size_;
}

const utils::ByteVector& Message::GetMessageAsBytes() const {
  if (shared_msg_) {
    return *shared_msg_;
  } else {
    return msg_;
  }
}

utils::SharedByteVector Message::GetMessageAsSharedBytes() {
  if (!shared_msg_) {
    shared_msg_ = utils::MakeSharedByteVector(std::move(msg_));
  }

  return shared_msg_;
}

void Message::PopulateMsg(boost::asio::const_buffer header,
                          boost::asio::const_buffer user_data,
                          api::Encoding user_enc) {
  auto header_raw = static_cast<const utils::Byte*>(header.data());
  auto user_data_raw = static_cast<const utils::Byte*>(user_data.data());

  switch (user_enc) {
    case api::Encoding::kJson: {
      utils::ByteVector data;
      if (user_data.size() > 0) {
        data = CheckAndConvertUserDataFromJsonToMsgPack(user_data);
      }

      msg_.reserve(1 + header.size() + data.size());
      msg_.push_back(type_);
      msg_.insert(msg_.end(), header_raw, header_raw + header.size());
      msg_.insert(msg_.end(), data.begin(), data.end());
      break;
    }

    case api::Encoding::kMsgPack:
      if (user_data.size() > 0) {
        CheckUserDataIsValidMsgPack(user_data);
      }

      msg_.reserve(1 + header.size() + user_data.size());
      msg_.push_back(type_);
      msg_.insert(msg_.end(), header_raw, header_raw + header.size());
      msg_.insert(msg_.end(), user_data_raw, user_data_raw + user_data.size());
      break;

    default:
      YOGI_NEVER_REACHED;
  }
}

utils::ByteVector Message::CheckAndConvertUserDataFromJsonToMsgPack(
    boost::asio::const_buffer user_data) {
  auto data = static_cast<const char*>(user_data.data());
  if (data[user_data.size() - 1] != '\0') {
    throw api::DescriptiveError(YOGI_ERR_PARSING_JSON_FAILED)
        << "Unterminated string";
  }

  try {
    auto json = nlohmann::json::parse(data);
    return nlohmann::json::to_msgpack(json);
  } catch (const std::exception& e) {
    throw api::DescriptiveError(YOGI_ERR_PARSING_JSON_FAILED) << e.what();
  }
}

void Message::CheckUserDataIsValidMsgPack(boost::asio::const_buffer user_data) {
  internal::MsgPackCheckVisitor visitor;
  bool ok = msgpack::parse(static_cast<const char*>(user_data.data()),
                           user_data.size(), visitor);
  if (!ok) {
    throw api::DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK)
        << "msgpack::parse() returned false";
  }
}

}  // namespace network

std::ostream& operator<<(std::ostream& os, const network::Message& msg) {
  os << '[';
  os << msg.GetType() << ", ";
  os << msg.GetHeader() << ", ";
  os << msg.GetUserDataSize() << " bytes user data";
  os << ']';

  return os;
}
