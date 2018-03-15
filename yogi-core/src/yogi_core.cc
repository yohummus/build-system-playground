#include "config.h"
#include "../include/yogi_core.h"
#include "errors.h"

YOGI_API const char* YOGI_GetVersion()
{
  return YOGI_HDR_VERSION;
}

YOGI_API const char* YOGI_GetErrorString(int err)
{
  return GetErrorString(err);
}
