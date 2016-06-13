#include <gtest/gtest.h>
#include "chasky/core/common/status.h"
using namespace chasky;

TEST(Status, init) {
  Status empty_status;
  ASSERT_TRUE(empty_status.ok());
}

TEST(Status, Update) {
  Status empty_status;
  Status error_status(error::Code::UNIMPLEMENTED, "");
  empty_status.Update(error_status);
  ASSERT_FALSE(empty_status.ok());
}

TEST(Status, Equal) {
  Status error_status(error::Code::UNIMPLEMENTED, "hel");
  Status error_status1(error::Code::UNIMPLEMENTED, "hel");
  Status error_status2(error::Code::INVALID_ARGUMENT, "hel");
  ASSERT_TRUE(error_status == error_status1);
  ASSERT_TRUE(error_status != error_status2);
}

TEST(Status, CHECK) {
  Status empty_status;
  CH_CHECK_OK(empty_status);
}
