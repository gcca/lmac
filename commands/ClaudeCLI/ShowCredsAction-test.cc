#include <gtest/gtest.h>

#include "ShowCredsAction.hpp"

TEST(ShowCredsActionTest, PrintsCredentials) {
  testing::internal::CaptureStdout();
  int result = ShowCredsAction([]() { return std::string("my-secret-token"); }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(out, "my-secret-token\n");
}

TEST(ShowCredsActionTest, ReaderErrorReturnsError) {
  testing::internal::CaptureStderr();
  int result = ShowCredsAction([]() -> std::string {
    throw std::runtime_error("not found");
  }).Run();
  std::string err = testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
  EXPECT_NE(err.find("not found"), std::string::npos);
}
