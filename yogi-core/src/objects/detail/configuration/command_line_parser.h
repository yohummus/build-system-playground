#pragma once

#include "../../../config.h"
#include "../../../utils/types.h"
#include "../../../../../3rd_party/json/json.hpp"

#include <boost/program_options.hpp>
#include <vector>
#include <string>

namespace objects {
namespace detail {

class CommandLineParser {
 public:
  enum CommandLineOptions {
    kNoOptions = YOGI_CLO_NONE,
    kLoggingOptions = YOGI_CLO_LOGGING,
    kBranchNameOption = YOGI_CLO_BRANCH_NAME,
    kBranchDescriptionOption = YOGI_CLO_BRANCH_DESCRIPTION,
    kBranchNetworkOption = YOGI_CLO_BRANCH_NETWORK,
    kBranchPasswordOption = YOGI_CLO_BRANCH_PASSWORD,
    kBranchPathOption = YOGI_CLO_BRANCH_PATH,
    kBranchAdvaddrOption = YOGI_CLO_BRANCH_ADVADDR,
    kBranchAdvportOption = YOGI_CLO_BRANCH_ADVPORT,
    kBranchAdvintOption = YOGI_CLO_BRANCH_ADVINT,
    kBranchTimeoutOption = YOGI_CLO_BRANCH_TIMEOUT,
    kFileOption = YOGI_CLO_FILES,
    kFileRequiredOption = YOGI_CLO_FILES_REQUIRED,
    kOverrideOption = YOGI_CLO_OVERRIDES,
    kVariableOption = YOGI_CLO_VARIABLES,
    kAllOptions = YOGI_CLO_ALL,
  };

  CommandLineParser(int argc, const char* const* argv,
                    CommandLineOptions options);

  nlohmann::json Parse();
  const std::string& GetLastErrorString() const { return err_description_; }

 private:
  void AddHelpOptions();
  void AddLoggingOptions();
  void AddBranchOptions();
  void AddFileOptions();
  void AddOverrideOptions();
  void AddVariableOptions();

  void PopulateVariablesMap();
  void HandleHelpOptions();
  void ExtractOptions();

  void LogFileNotifier(const std::string& val);
  void LogConsoleNotifier(const std::string& val);
  void LogVerbosityNotifier(const std::vector<std::string>& val);
  void FileNotifier(const std::vector<std::string>& val);
  void OverrideNotifier(const std::vector<std::string>& val);
  void VariableNotifier(const std::vector<std::string>& val);

  const int argc_;
  const char* const* const argv_;
  const CommandLineOptions options_;
  std::string err_description_;

  boost::program_options::options_description visible_options_;
  boost::program_options::options_description hidden_options_;
  boost::program_options::positional_options_description positional_options_;
  boost::program_options::variables_map vm_;
  nlohmann::json json_;

  std::vector<std::string> config_files_;
  std::vector<nlohmann::json> overrides_;
};

YOGI_DEFINE_FLAG_OPERATORS(CommandLineParser::CommandLineOptions);

}  // namespace detail
}  // namespace objects
