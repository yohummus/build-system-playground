#include "config.h"
#include "../include/yogi_core.h"
#include "api/constants.h"
#include "api/error.h"
#include "objects/context.h"

#include <stdexcept>

#define CHECK_PARAM(cond) {                    \
  if (!(cond))                                 \
    return YOGI_ERR_INVALID_PARAM;             \
}

#define CHECK_BUFFER_PARAMS(buffer, size) {     \
  CHECK_PARAM(buffer != nullptr || size == 0); \
  CHECK_PARAM(size >= 0);                      \
}

#define CATCH_AND_RETURN                       \
  catch (const api::Error& err) {              \
    return err.error_code();                   \
  }                                            \
  catch (const std::bad_alloc&) {              \
    return YOGI_ERR_BAD_ALLOC;                 \
  }                                            \
  catch (...) {                                \
    return YOGI_ERR_UNKNOWN;                   \
  }                                            \
  return YOGI_OK;

YOGI_API const char* YOGI_GetVersion() {
  return api::kVersionNumber;
}

YOGI_API const char* YOGI_GetErrorString(int err) {
  return api::Error(err).what();
}

YOGI_API int YOGI_GetConstant(void* dest, int constant) {
  CHECK_PARAM(dest != nullptr);

  try {
    api::GetConstant(dest, constant);
  }
  CATCH_AND_RETURN;
}

YOGI_API int YOGI_CreateContext(void** context) {
  CHECK_PARAM(context != nullptr);

  try {
    auto obj = std::make_shared<objects::Context>();
  }
  CATCH_AND_RETURN;
}
