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
#include "../../context.h"
#include "../../logger.h"

namespace objects {
namespace detail {

class BroadcastManager final
    : public std::enable_shared_from_this<BroadcastManager> {
 public:
  BroadcastManager(ContextPtr context);
  virtual ~BroadcastManager();

 private:
  static const LoggerPtr logger_;

  const ContextPtr context_;
};

typedef std::shared_ptr<BroadcastManager> BroadcastManagerPtr;

}  // namespace detail
}  // namespace objects
