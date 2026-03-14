#include <gtest/gtest.h>

#include "ClaudeDeskCommand.hpp"

TEST(ClaudeDeskCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeDeskCommand{}.Run({});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("claude-desk"), std::string::npos);
}

TEST(ClaudeDeskCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeDeskCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("claude-desk"), std::string::npos);
}

TEST(ClaudeDeskCommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = ClaudeDeskCommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
