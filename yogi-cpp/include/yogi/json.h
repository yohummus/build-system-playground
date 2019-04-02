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

#ifndef YOGI_JSON_H
#define YOGI_JSON_H

//! \file
//!
//! Includes Niels Lohmann's JSON library.

#ifdef NLOHMANN_JSON_VERSION_MAJOR
#if NLOHMANN_JSON_VERSION_MAJOR != 3
#error "Incompatible version of Niels Lohmann's JSON library has been included"
#endif
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wsign-conversion"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "3rd_party/nlohmann/json.hpp"

#pragma GCC diagnostic pop

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif

namespace yogi {

/// Alias for Niels Lohmann's JSON library.
using Json = ::nlohmann::json;

}  // namespace yogi

#endif  // YOGI_JSON_H
