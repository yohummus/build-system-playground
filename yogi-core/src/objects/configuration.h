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
#include "../api/object.h"
#include "../api/error.h"
#include "../utils/types.h"
#include "detail/command_line_parser.h"
#include "logger.h"

#include <nlohmann/json.hpp>
#include <mutex>

namespace objects {

class Configuration
    : public api::ExposedObjectT<Configuration,
                                 api::ObjectType::kConfiguration> {
 public:
  enum ConfigurationFlags {
    kNoFlags = YOGI_CFG_NONE,
    kDisableVariables = YOGI_CFG_DISABLE_VARIABLES,
    kMutableCmdLine = YOGI_CFG_MUTABLE_CMD_LINE,
    kAllFlags = kDisableVariables | kMutableCmdLine,
  };

  using CommandLineOptions = detail::CommandLineParser::CommandLineOptions;

  Configuration(ConfigurationFlags flags);

  void UpdateFromCommandLine(int argc, const char* const* argv,
                             CommandLineOptions options, std::string* err_desc);
  void UpdateFromString(const std::string& json_str, std::string* err_desc);
  void UpdateFromFile(const std::string& filename, std::string* err_desc);
  std::string Dump(bool resolve_variables, int indentation_width) const;
  void WriteToFile(const std::string& filename, bool resolve_variables,
                   int indentation_width) const;

 private:
  static void CheckCircularVariableDependency(const std::string& var_ref,
                                              const nlohmann::json& var_val,
                                              std::string* err_desc);
  static void ResolveVariablesSections(nlohmann::json* vars,
                                       std::string* err_desc);
  static void ResolveSingleVariable(nlohmann::json* elem,
                                    const std::string& var_ref,
                                    const nlohmann::json& var_val);
  static nlohmann::json ResolveVariables(const nlohmann::json& unresolved_json,
                                         std::string* err_desc);
  template <typename Fn>
  static void WalkAllElements(nlohmann::json* json, Fn fn);

  static void CheckVariablesOnlyUsedInValues(nlohmann::json* json,
                                             std::string* err_desc);
  static void CheckAllVariablesAreResolved(nlohmann::json* json,
                                           std::string* err_desc);

  void VerifyAndMerge(const nlohmann::json& json_to_merge,
                      const nlohmann::json& immutable_json,
                      std::string* err_desc);

  static const LoggerPtr logger_;

  const bool variables_supported_;
  const bool mutable_cmdline_;
  mutable std::mutex mutex_;

  nlohmann::json json_;
  nlohmann::json immutable_json_;
};

typedef std::shared_ptr<Configuration> ConfigurationPtr;

YOGI_DEFINE_FLAG_OPERATORS(Configuration::ConfigurationFlags)

}  // namespace objects
