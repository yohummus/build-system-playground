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

#include "helpers.h"

#include <string.h>
#include <boost/algorithm/string.hpp>
#include <regex>

std::chrono::nanoseconds ExtractDuration(const nlohmann::json& json,
                                         const char* key,
                                         long long defaultValue) {
  float seconds = json.value(key, static_cast<float>(defaultValue) / 1e9f);
  if (seconds == -1) {
    return (std::chrono::nanoseconds::max)();
  }
  else if (seconds < 0) {
    return (std::chrono::nanoseconds::max)();
    // TODO: Throw exception with additional information
  }

  auto ns = static_cast<long long>(seconds * 1e9f);
  return std::chrono::nanoseconds(ns);
}

bool IsExactlyOneBitSet(int bit_field) {
  auto x = static_cast<unsigned int>(bit_field);
  return x && !(x & (x - 1));
}

bool IsTimeFormatValid(const std::string& fmt) {
  if (fmt.empty()) return false;

  std::regex re("%([^YmdFHMST369]|$)");
  return !std::regex_search(fmt, re);
}

bool IsLogFormatValid(std::string fmt) {
  if (fmt.empty()) return false;

  boost::replace_all(fmt, "$$", "");
  std::regex re("\\$([^tPTsmflc<>]|$)");
  return !std::regex_search(fmt, re);
}

void CopyUuidToUserBuffer(const boost::uuids::uuid& uuid, void* buffer) {
  if (buffer == nullptr) return;
  std::memcpy(buffer, &uuid, uuid.size());
}

bool CopyStringToUserBuffer(const std::string& str, char* buffer,
                            int buffer_size) {
  if (buffer == nullptr) return true;

  auto n = std::min(str.size() + 1, static_cast<std::size_t>(buffer_size));
  std::strncpy(buffer, str.c_str(), n);
  if (str.size() + 1 > n) {
    buffer[buffer_size - 1] = '\0';
    return false;
  }

  return true;
}
