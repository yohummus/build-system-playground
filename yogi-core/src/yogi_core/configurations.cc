#include "macros.h"
#include "helpers.h"
#include "../objects/configuration.h"

#include <iostream>

YOGI_API int YOGI_ConfigurationCreate(void** config, int flags) {
  using Flags = objects::Configuration::ConfigurationFlags;

  CHECK_PARAM(config != nullptr);
  CHECK_FLAGS(flags, Flags::kAllFlags);

  try {
    auto cfg =
        objects::Configuration::Create(ConvertFlags(flags, Flags::kNoFlags));
    *config = api::ObjectRegister::Register(cfg);
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_ConfigurationUpdateFromCommandLine(void* config, int argc,
                                                     const char* const* argv,
                                                     int options, char* err,
                                                     int errsize) {
  using Options = objects::Configuration::CommandLineOptions;

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(argc > 0);
  CHECK_PARAM(argv != nullptr);
  CHECK_FLAGS(options, Options::kAllOptions);
  CHECK_PARAM(err == nullptr || errsize > 0);

  try {
    auto cfg = api::ObjectRegister::Get<objects::Configuration>(config);

    std::string err_desc;
    try {
      cfg->UpdateFromCommandLine(
          argc, argv, ConvertFlags(options, Options::kNoOptions), &err_desc);
    } catch (...) {
      CopyStringToUserBuffer(err_desc, err, errsize);
      throw;
    }

    CopyStringToUserBuffer(err_desc, err, errsize);
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_ConfigurationUpdateFromJson(void* config, const char* json,
                                              char* err, int errsize) {
  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(json != nullptr);
  CHECK_PARAM(err == nullptr || errsize > 0);

  try {
    auto cfg = api::ObjectRegister::Get<objects::Configuration>(config);

    std::string err_desc;
    try {
      cfg->UpdateFromString(json, &err_desc);
    } catch (...) {
      CopyStringToUserBuffer(err_desc, err, errsize);
      throw;
    }

    CopyStringToUserBuffer(err_desc, err, errsize);
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_ConfigurationUpdateFromFile(void* config,
                                              const char* filename, char* err,
                                              int errsize) {
  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(filename != nullptr);
  CHECK_PARAM(err == nullptr || errsize > 0);

  try {
    auto cfg = api::ObjectRegister::Get<objects::Configuration>(config);

    std::string err_desc;
    try {
      cfg->UpdateFromFile(filename, &err_desc);
    } catch (...) {
      CopyStringToUserBuffer(err_desc, err, errsize);
      throw;
    }

    CopyStringToUserBuffer(err_desc, err, errsize);
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_ConfigurationDump(void* config, char* json, int jsonsize,
                                    int resvars, int indent) {
  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(json != nullptr);
  CHECK_PARAM(jsonsize > 0);
  CHECK_PARAM(resvars == YOGI_TRUE || resvars == YOGI_FALSE);
  CHECK_PARAM(indent >= -1);

  try {
    auto cfg = api::ObjectRegister::Get<objects::Configuration>(config);
    auto str = cfg->Dump(resvars == YOGI_TRUE, indent);
    if (!CopyStringToUserBuffer(str, json, jsonsize)) {
      return YOGI_ERR_BUFFER_TOO_SMALL;
    }
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_ConfigurationWriteToFile(void* config, const char* filename,
                                           int resvars, int indent) {
  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(filename != nullptr);
  CHECK_PARAM(resvars == YOGI_TRUE || resvars == YOGI_FALSE);
  CHECK_PARAM(indent >= -1);

  try {
    auto cfg = api::ObjectRegister::Get<objects::Configuration>(config);
    cfg->WriteToFile(filename, resvars == YOGI_TRUE, indent);
  }
  CATCH_AND_RETURN;
}
