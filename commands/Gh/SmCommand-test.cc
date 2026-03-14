#include <gtest/gtest.h>

#include "SmCommand.hpp"

TEST(SmCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = SmCommand{}.Run({});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}

TEST(SmCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = SmCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("sm"), std::string::npos);
}

TEST(SmCommandTest, MissingSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = SmCommand{}.Run({"--date", "2026-01-15"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(SmCommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = SmCommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
