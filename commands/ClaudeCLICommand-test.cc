#include <gtest/gtest.h>

#include "ClaudeCLICommand.hpp"

TEST(ClaudeCLICommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLICommand{}.Run({});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("claude-cli"), std::string::npos);
}

TEST(ClaudeCLICommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLICommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("list"), std::string::npos);
}

TEST(ClaudeCLICommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = ClaudeCLICommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
