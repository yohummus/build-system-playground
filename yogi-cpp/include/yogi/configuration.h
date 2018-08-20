#pragma once

#include "object.h"
#include "io.h"
#include "internal/flags.h"
#include "internal/query_string.h"

#include <memory>

namespace yogi {

YOGI_DEFINE_API_FN(int, YOGI_ConfigurationCreate, (void** config, int flags))
YOGI_DEFINE_API_FN(int, YOGI_ConfigurationUpdateFromCommandLine,
                   (void* config, int argc, const char* const* argv,
                    int options, char* err, int errsize))
YOGI_DEFINE_API_FN(int, YOGI_ConfigurationUpdateFromJson,
                   (void* config, const char* json, char* err, int errsize))
YOGI_DEFINE_API_FN(int, YOGI_ConfigurationUpdateFromFile,
                   (void* config, const char* filename, char* err, int errsize))
YOGI_DEFINE_API_FN(int, YOGI_ConfigurationDump,
                   (void* config, char* json, int jsonsize, int resvars,
                    int indent))
YOGI_DEFINE_API_FN(int, YOGI_ConfigurationWriteToFile,
                   (void* config, const char* filename, int resvars,
                    int indent))

/// Flags used to change a configuration object's behaviour.
enum class ConfigurationFlags {
  /// No flags.
  kNone = 0,

  /// Disables support for variables in the configuration.
  kDisableVariables = (1 << 0),

  /// Makes configuration options given directly on the command line
  /// overridable.
  kMutableCmdLine = (1 << 1),
};

YOGI_DEFINE_FLAG_OPERATORS(ConfigurationFlags)

template <>
inline std::string ToString<ConfigurationFlags>(
    const ConfigurationFlags& flags) {
  switch (flags) {
    YOGI_TO_STRING_ENUM_CASE(ConfigurationFlags, kNone)
    YOGI_TO_STRING_ENUM_CASE(ConfigurationFlags, kDisableVariables)
    YOGI_TO_STRING_ENUM_CASE(ConfigurationFlags, kMutableCmdLine)
  }

  std::string s;
  YOGI_TO_STRING_FLAG_APPENDER(flags, ConfigurationFlags, kDisableVariables)
  YOGI_TO_STRING_FLAG_APPENDER(flags, ConfigurationFlags, kMutableCmdLine)
  return s.substr(3);
}

/// Flags used to adjust how command line options are parsed.
enum class CommandLineOptions {
  /// No options.
  kNone = 0,

  /// Include logging configuration for file logging.
  kLogging = (1 << 0),

  /// Include branch name configuration.
  kBranchName = (1 << 1),

  /// Include branch description configuration.
  kBranchDescription = (1 << 2),

  /// Include network name configuration.
  kBranchNetwork = (1 << 3),

  /// Include network password configuration.
  kBranchPassword = (1 << 4),

  /// Include branch path configuration.
  kBranchPath = (1 << 5),

  /// Include branch advertising address configuration.
  kBranchAdvAddr = (1 << 6),

  /// Include branch advertising port configuration.
  kBranchAdvPort = (1 << 7),

  /// Include branch advertising interval configuration.
  kBranchAdvInt = (1 << 8),

  /// Include branch timeout configuration.
  kBranchTimeout = (1 << 9),

  /// Parse configuration files given on the command line.
  kFiles = (1 << 10),

  /// Same as the Files option but at least one configuration file must be
  /// given.
  kFilesRequired = (1 << 11),

  /// Allow overriding arbitrary configuration sections.
  kOverrides = (1 << 12),

  /// Allow setting variables via a dedicated switch.
  kVariables = (1 << 13),

  /// Combination of all branch flags.
  kBranchAll = kBranchName | kBranchDescription | kBranchNetwork |
               kBranchPassword | kBranchPath | kBranchAdvAddr | kBranchAdvPort |
               kBranchAdvInt | kBranchTimeout,

  /// Combination of all flags.
  kAll =
      kLogging | kBranchAll | kFiles | kFilesRequired | kOverrides | kVariables,
};

YOGI_DEFINE_FLAG_OPERATORS(CommandLineOptions)

template <>
inline std::string ToString<CommandLineOptions>(
    const CommandLineOptions& options) {
  switch (options) {
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kNone)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kLogging)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchName)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchDescription)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchNetwork)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchPassword)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchPath)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchAdvAddr)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchAdvPort)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchAdvInt)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchTimeout)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kFiles)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kFilesRequired)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kOverrides)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kVariables)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kBranchAll)
    YOGI_TO_STRING_ENUM_CASE(CommandLineOptions, kAll)
  }

  std::string s;
  YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kLogging)
  if ((options & CommandLineOptions::kBranchAll) ==
      CommandLineOptions::kBranchAll) {
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchAll)
  } else {
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchName)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions,
                                 kBranchDescription)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchNetwork)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchPassword)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchPath)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchAdvAddr)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchAdvPort)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchAdvInt)
    YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kBranchTimeout)
  }

  YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kFiles)
  YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kFilesRequired)
  YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kOverrides)
  YOGI_TO_STRING_FLAG_APPENDER(options, CommandLineOptions, kVariables)
  return s.substr(3);
}

class Configuration;
using ConfigurationPtr = std::shared_ptr<Configuration>;

/// A configuration represents a set of parameters that usually remain constant
/// throughout the runtime of a program. Parameters can come from different
/// sources such as the command line or a file. Configurations are used for
/// other parts of the library such as application objects, however, they are
/// also intended to store user-defined parameters.
class Configuration : public ObjectT<Configuration> {
 public:
  /// Creates a configuration.
  ///
  /// \param flags Flags for behaviour adjustments.
  ///
  /// \returns The created configuration.
  static ConfigurationPtr Create(
      ConfigurationFlags flags = ConfigurationFlags::kNone) {
    return ConfigurationPtr(new Configuration(flags));
  }

  /// Returns the flags set for the configuration.
  ///
  /// \return Flags set for the configuration.
  ConfigurationFlags GetFlags() const { return flags_; }

  /// Updates the configuration from command line options.
  ///
  /// If parsing the command line, files or any given JSON string fails, or
  /// if help is requested (e.g. by using the --help switch) then a
  /// DescriptiveFailure exception will be raised containing detailed
  /// information about the error or the help text.
  ///
  /// \param argc    Number of command line arguments in \p argv.
  /// \param argv    Command line arguments as passed to main().
  /// \param options Options to provide on the command line.
  void UpdateFromCommandLine(
      int argc, const char* const* argv,
      CommandLineOptions options = CommandLineOptions::kNone) {
    internal::CheckDescriptiveErrorCode([&](auto err, auto size) {
      return internal::YOGI_ConfigurationUpdateFromCommandLine(
          GetHandle(), argc, argv, static_cast<int>(options), err, size);
    });
  }

  /// Updates the configuration from a JSON object.
  ///
  /// If parsing fails then a DescriptiveFailure exception will be raised
  /// containing detailed information about the error.
  ///
  /// \tparam String Type of the \p json parameter.
  ///
  /// \param json Serialized JSON object.
  template <typename String>
  void UpdateFromJson(String&& json) {
    internal::CheckDescriptiveErrorCode([&](auto err, auto size) {
      return internal::YOGI_ConfigurationUpdateFromJson(
          GetHandle(), internal::ToCoreString(json), err, size);
    });
  }

  /// Updates the configuration from a JSON file.
  ///
  /// If parsing the file fails then a DescriptiveFailure exception will be
  /// raised containing detailed information about the error.
  ///
  /// \tparam String Type of the \p filename parameter.
  ///
  /// \param filename Path to the JSON file.
  template <typename String>
  void UpdateFromFile(String&& filename) {
    internal::CheckDescriptiveErrorCode([&](auto err, auto size) {
      return internal::YOGI_ConfigurationUpdateFromFile(
          GetHandle(), internal::ToCoreString(filename), err, size);
    });
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use for
  ///                          indentation; must be >= 0.
  ///
  /// \returns The configuration serialized to a string.
  std::string Dump(bool resolve_variables, int indentation) const {
    if (indentation < 0) {
      throw Failure(ErrorCode::kInvalidParam);
    }

    return DumpImpl(resolve_variables, indentation);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \param resolve_variables Resolve all configuration variables.
  ///
  /// \returns The configuration serialized to a string.
  std::string Dump(bool resolve_variables) const {
    return DumpImpl(resolve_variables, -1);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// Configuration variables get resolved if the configuration supports them.
  ///
  /// \param indentation Number of space characters to use for indentation;
  ///                    must be >= 0.
  ///
  /// \returns The configuration serialized to a string.
  std::string Dump(int indentation) const {
    if (indentation < 0) {
      throw Failure(ErrorCode::kInvalidParam);
    }

    return DumpImpl((flags_ & ConfigurationFlags::kDisableVariables) ==
                        ConfigurationFlags::kNone,
                    indentation);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// Configuration variables get resolved if the configuration supports them.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \returns The configuration serialized to a string.
  std::string Dump() const {
    return DumpImpl((flags_ & ConfigurationFlags::kDisableVariables) ==
                        ConfigurationFlags::kNone,
                    -1);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// \tparam String Type of the \p filename parameter.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use for
  ///                          indentation; must be >= 0.
  template <typename String>
  void WriteToFile(String&& filename, bool resolve_variables,
                   int indentation) const {
    if (indentation < 0) {
      throw Failure(ErrorCode::kInvalidParam);
    }

    WriteToFileImpl(std::forward<String>(filename), resolve_variables,
                    indentation);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \tparam String Type of the \p filename parameter.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  template <typename String>
  void WriteToFile(String&& filename, bool resolve_variables) const {
    WriteToFileImpl(std::forward<String>(filename), resolve_variables, -1);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// Configuration variables get resolved if the configuration supports them.
  ///
  /// \tparam String Type of the \p filename parameter.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use for
  ///                          indentation; must be >= 0.
  template <typename String>
  void WriteToFile(String&& filename, int indentation) const {
    if (indentation < 0) {
      throw Failure(ErrorCode::kInvalidParam);
    }

    WriteToFileImpl(std::forward<String>(filename),
                    (flags_ & ConfigurationFlags::kDisableVariables) ==
                        ConfigurationFlags::kNone,
                    indentation);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// Configuration variables get resolved if the configuration supports them.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \tparam String Type of the \p filename parameter.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use for
  ///                          indentation; must be >= 0.
  template <typename String>
  void WriteToFile(String&& filename) const {
    WriteToFileImpl(std::forward<String>(filename),
                    (flags_ & ConfigurationFlags::kDisableVariables) ==
                        ConfigurationFlags::kNone,
                    -1);
  }

 private:
  Configuration(ConfigurationFlags flags)
      : ObjectT(internal::CallApiCreate(internal::YOGI_ConfigurationCreate,
                                        static_cast<int>(flags)),
                {}),
        flags_(flags) {}

  std::string DumpImpl(bool resolve_variables, int indentation) const {
    return internal::QueryString([&](auto json, auto size) {
      return internal::YOGI_ConfigurationDump(
          GetHandle(), json, size, resolve_variables ? 1 : 0, indentation);
    });
  }

  template <typename String>
  void WriteToFileImpl(String&& filename, bool resolve_variables,
                       int indentation) const {
    int res = internal::YOGI_ConfigurationWriteToFile(
        GetHandle(), internal::ToCoreString(filename),
        resolve_variables ? 1 : 0, indentation);
    internal::CheckErrorCode(res);
  }

  const ConfigurationFlags flags_;
};

}  // namespace yogi