#pragma once

#include "io.h"
#include "errors.h"

#include <sstream>

#define YOGI_DEFINE_OSTREAM_OPERATOR(declaration_type, type)                 \
  namespace yogi {                                                           \
    declaration_type type;                                                   \
  }                                                                          \
  inline std::ostream& operator<<(std::ostream& os, const yogi::type& val) { \
    os << yogi::ToString(val);                                               \
    return os;                                                               \
  }

YOGI_DEFINE_OSTREAM_OPERATOR(enum class, ErrorCode);
YOGI_DEFINE_OSTREAM_OPERATOR(class, Result);

#undef YOGI_DEFINE_OSTREAM_OPERATOR
