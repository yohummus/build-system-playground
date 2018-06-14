#pragma once

#include "../config.h"
#include "../../include/yogi_core.h"
#include "../api/error.h"

#include <stdexcept>
#include <regex>

#define CHECK_PARAM(cond)                       \
  {                                             \
    if (!(cond)) return YOGI_ERR_INVALID_PARAM; \
  }

#define CHECK_BUFFER_PARAMS(buffer, size)        \
  {                                              \
    CHECK_PARAM(buffer != nullptr || size == 0); \
    CHECK_PARAM(size >= 0);                      \
  }

#define CHECK_FLAGS(flags, all_flags) \
  { CHECK_PARAM(!(flags & ~all_flags)); }

#ifdef NDEBUG
#define CATCH_AND_RETURN_INTERNAL_ERRORS                               \
  catch (const std::exception& e) {                                    \
    fprintf(stderr, "%s:%i: INTERNAL ERROR: %s\n", __FILE__, __LINE__, \
            e.what());                                                 \
    return YOGI_ERR_UNKNOWN;                                           \
  }                                                                    \
  catch (...) {                                                        \
    fprintf(stderr, "%s:%i: INTERNAL ERROR: %s\n", __FILE__, __LINE__, \
            "Unknown error");                                          \
    return YOGI_ERR_UNKNOWN;                                           \
  }
#else
#define CATCH_AND_RETURN_INTERNAL_ERRORS
#endif

#define CATCH_AND_RETURN            \
  catch (const api::Error& err) {   \
    return err.error_code();        \
  }                                 \
  catch (const std::bad_alloc&) {   \
    return YOGI_ERR_BAD_ALLOC;      \
  }                                 \
  catch (const std::regex_error&) { \
    return YOGI_ERR_INVALID_REGEX;  \
  }                                 \
  CATCH_AND_RETURN_INTERNAL_ERRORS  \
  return YOGI_OK;