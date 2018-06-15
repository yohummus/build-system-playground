#include "../common.h"
#include "../../src/objects/detail/command_line_parser.h"

#include <boost/filesystem.hpp>

using objects::detail::CommandLineParser;
namespace fs = boost::filesystem;

class CommandLineParserTest : public Test {
 protected:
  virtual void SetUp() override {
    temp_path = fs::temp_directory_path() / fs::unique_path();
    fs::create_directory(temp_path);
    old_working_dir = fs::current_path();
    fs::current_path(temp_path);
  }

  virtual void TearDown() override {
    fs::current_path(old_working_dir);
    fs::remove_all(temp_path);
  }

  nlohmann::json CheckParsingSucceeds(
      const CommandLine& cmdline, CommandLineParser::CommandLineOptions options,
      const char* section_name) {
    CommandLineParser parser(cmdline.argc, cmdline.argv, options);
    EXPECT_NO_THROW(parser.Parse()) << parser.GetLastErrorString();

    auto section = parser.GetDirectConfiguration()[section_name];
    EXPECT_FALSE(section.empty());

    return section;
  }

  void CheckParsingFailsWithNoOptions(const CommandLine& cmdline) {
    CommandLineParser parser(cmdline.argc, cmdline.argv,
                             CommandLineParser::kNoOptions);
    EXPECT_THROW_ERROR(parser.Parse(), YOGI_ERR_PARSING_CMDLINE_FAILED);
    EXPECT_FALSE(parser.GetLastErrorString().empty());
  }

  template <typename T>
  void CheckParsingSingleValue(const char* cmdline_option, T val,
                               CommandLineParser::CommandLineOptions options,
                               const char* section_name, const char* key) {
    CommandLine cmdline{
        cmdline_option,
        std::to_string(val),
    };

    auto section = CheckParsingSucceeds(cmdline, options, section_name);
    ASSERT_TRUE(!!section.count(key));
    EXPECT_FLOAT_EQ(section[key].get<T>(), val);

    CheckParsingFailsWithNoOptions(cmdline);
  }

  void CheckParsingSingleValue(const char* cmdline_option, const char* val,
                               CommandLineParser::CommandLineOptions options,
                               const char* section_name, const char* key) {
    CommandLine cmdline{
        cmdline_option,
        val,
    };

    auto section = CheckParsingSucceeds(cmdline, options, section_name);
    ASSERT_TRUE(!!section.count(key));
    EXPECT_EQ(section[key].get<std::string>(), val);

    CheckParsingFailsWithNoOptions(cmdline);
  }

  fs::path old_working_dir;
  fs::path temp_path;
};

TEST_F(CommandLineParserTest, HelpOption) {
  // clang-format off
  CommandLine cmdline{
    "--help",
  };
  // clang-format on

  CommandLineParser parser(cmdline.argc, cmdline.argv,
                           CommandLineParser::kNoOptions);

  EXPECT_THROW_ERROR(parser.Parse(), YOGI_ERR_HELP_REQUESTED);
  EXPECT_FALSE(parser.GetLastErrorString().empty());
}

TEST_F(CommandLineParserTest, HelpLoggingOption) {
  // clang-format off
  CommandLine cmdline{
    "--help-logging",
  };
  // clang-format on

  CommandLineParser parser(cmdline.argc, cmdline.argv,
                           CommandLineParser::kLoggingOptions);

  EXPECT_THROW_ERROR(parser.Parse(), YOGI_ERR_HELP_REQUESTED);
  EXPECT_FALSE(parser.GetLastErrorString().empty());

  CheckParsingFailsWithNoOptions(cmdline);
}

TEST_F(CommandLineParserTest, LoggingOptions) {
  // clang-format off
  CommandLine cmdline{
    "--log-file", "/tmp/logfile.txt",
    "--log-console=STDOUT",
    "--log-colour",
    "--log-fmt", "some entry format",
    "--log-time-fmt", "some time format",
    "--log-verbosity", "Yogi.*=WARNING",
    "--log-verbosity=App=DEBUG",
  };
  // clang-format on

  auto section = CheckParsingSucceeds(
      cmdline, CommandLineParser::kLoggingOptions, "logging");

  EXPECT_EQ(section.value("file", "NOT FOUND"), "/tmp/logfile.txt");
  EXPECT_EQ(section.value("console", "NOT FOUND"), "STDOUT");
  EXPECT_EQ(section.value("colour", false), true);
  EXPECT_EQ(section.value("entry-format", "NOT FOUND"), "some entry format");
  EXPECT_EQ(section.value("time-format", "NOT FOUND"), "some time format");

  auto verbosities = section["verbosity"];
  EXPECT_FALSE(verbosities.empty());

  EXPECT_EQ(verbosities.value("Yogi.*", "NOT FOUND"), "WARNING");
  EXPECT_EQ(verbosities.value("App", "NOT FOUND"), "DEBUG");

  CheckParsingFailsWithNoOptions(cmdline);
}

TEST_F(CommandLineParserTest, BranchNameOption) {
  CheckParsingSingleValue("--name", "My Branch",
                          CommandLineParser::kBranchNameOption, "branch",
                          "name");
}

TEST_F(CommandLineParserTest, BranchDescriptionOption) {
  CheckParsingSingleValue("--description", "Some text...",
                          CommandLineParser::kBranchDescriptionOption, "branch",
                          "description");
}

TEST_F(CommandLineParserTest, BranchNetworkOption) {
  CheckParsingSingleValue("--network", "General",
                          CommandLineParser::kBranchNetworkOption, "branch",
                          "network");
}

TEST_F(CommandLineParserTest, BranchPasswordOption) {
  CheckParsingSingleValue("--password", "Secret",
                          CommandLineParser::kBranchPasswordOption, "branch",
                          "password");
}

TEST_F(CommandLineParserTest, BranchPathOption) {
  CheckParsingSingleValue("--path", "/some/path",
                          CommandLineParser::kBranchPathOption, "branch",
                          "path");
}

TEST_F(CommandLineParserTest, BranchAdvertisingAddressOption) {
  CheckParsingSingleValue("--adv-addr", "0::0",
                          CommandLineParser::kBranchAdvAddressOption, "branch",
                          "advertising-address");
}

TEST_F(CommandLineParserTest, BranchAdvertisingPortOption) {
  CheckParsingSingleValue("--adv-port", 12345,
                          CommandLineParser::kBranchAdvPortOption, "branch",
                          "advertising-port");
}

TEST_F(CommandLineParserTest, BranchAdvertisingIntervalOption) {
  CheckParsingSingleValue("--adv-int", 3.5,
                          CommandLineParser::kBranchAdvIntervalOption, "branch",
                          "advertising-interval");
}

TEST_F(CommandLineParserTest, BranchTimeoutOption) {
  CheckParsingSingleValue("--timeout", 6.6,
                          CommandLineParser::kBranchTimeoutOption, "branch",
                          "timeout");
}

TEST_F(CommandLineParserTest, FileOption) {
  {
    fs::ofstream file("a.json");
    file << "{\"person\": {\"name\": \"Joe\", \"age\": 42}}";
  }
  {
    fs::ofstream file("bcde.json");
    file << "{\"person\": {\"age\": 88}}";
  }

  // clang-format off
  CommandLine cmdline{
    "a.json",
    "bc*.json",
  };
  // clang-format on

  CommandLineParser parser(cmdline.argc, cmdline.argv,
                           CommandLineParser::kFileOption);
  EXPECT_NO_THROW(parser.Parse()) << parser.GetLastErrorString();

  auto section = parser.GetFilesConfiguration()["person"];
  EXPECT_FALSE(section.empty());

  EXPECT_EQ(section.value("name", "NOT FOUND"), "Joe");
  EXPECT_EQ(section.value("age", -1), 88);

  CheckParsingFailsWithNoOptions(cmdline);
}

TEST_F(CommandLineParserTest, OverrideOption) {
  // clang-format off
  CommandLine cmdline{
    "--override", "{\"person\":{\"age\":42}}",
    "--o={\"person\":{\"name\":\"Joe\"}}",
    "-o", "/person/name=Marc",
  };
  // clang-format on

  auto section = CheckParsingSucceeds(
      cmdline, CommandLineParser::kOverrideOption, "person");

  EXPECT_EQ(section.value("age", -1), 42);
  EXPECT_EQ(section.value("name", "NOT FOUND"), "Marc");

  CheckParsingFailsWithNoOptions(cmdline);
}

TEST_F(CommandLineParserTest, VariableOptions) {
  // clang-format off
  CommandLine cmdline{
    "--var", "DIR=\"/usr/local\"",
    "--var=NAME=Yohummus",
    "--v=NUM=55",
    "--v", "PI=3.14",
  };
  // clang-format on

  auto section = CheckParsingSucceeds(
      cmdline, CommandLineParser::kVariableOption, "variables");

  EXPECT_EQ(section.value("DIR", "NOT FOUND"), "/usr/local");
  EXPECT_EQ(section.value("NAME", "NOT FOUND"), "Yohummus");
  EXPECT_EQ(section.value("NUM", -1), 55);
  EXPECT_FLOAT_EQ(section.value("PI", -1.0f), 3.14f);

  CheckParsingFailsWithNoOptions(cmdline);
}