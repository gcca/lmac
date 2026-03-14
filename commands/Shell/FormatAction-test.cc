#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "FormatAction.hpp"

using testing::HasSubstr;

TEST(FormatActionTest, InvalidFormatReturnsError) {
  testing::internal::CaptureStderr();
  int result = FormatAction("invalid").Run();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(FormatActionTest, FishFormatPrintsCompletions) {
  testing::internal::CaptureStdout();
  int result = FormatAction("fish").Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_THAT(out, HasSubstr("complete -c lmac"));
  EXPECT_THAT(out, HasSubstr("claude-desk"));
  EXPECT_THAT(out, HasSubstr("gh"));
  EXPECT_THAT(out, HasSubstr("sm"));
  EXPECT_THAT(out, HasSubstr("shell"));
}

TEST(FormatActionTest, ZshFormatReturnsSuccess) {
  EXPECT_EQ(FormatAction("zsh").Run(), 0);
}

TEST(FormatActionTest, BashFormatReturnsSuccess) {
  EXPECT_EQ(FormatAction("bash").Run(), 0);
}
