#include <gtest/gtest.h>

#include "ShellCommand.hpp"

TEST(ShellCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ShellCommand{}.Run({});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}

TEST(ShellCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ShellCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("shell"), std::string::npos);
}

TEST(ShellCommandTest, InvalidFormatReturnsError) {
  testing::internal::CaptureStderr();
  int result = ShellCommand{}.Run({"--format", "invalid"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(ShellCommandTest, FishFormatReturnsSuccess) {
  testing::internal::CaptureStdout();
  int result = ShellCommand{}.Run({"--format", "fish"});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}
