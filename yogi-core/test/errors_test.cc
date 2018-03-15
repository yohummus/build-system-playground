#include <gtest/gtest.h>
#include <yogi_core.h>
#include "../src/errors.h"

TEST(VersionTest, GetErrorString) {
  EXPECT_STREQ(GetErrorString(34), YOGI_GetErrorString(123));
  EXPECT_STREQ("Hello World", YOGI_GetErrorString(123));
}
