#include <gtest/gtest.h>

#include "GhCommand.hpp"

TEST(GhCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = GhCommand{}.Run({});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}

TEST(GhCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = GhCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("gh"), std::string::npos);
}

TEST(GhCommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = GhCommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(GhCommandTest, SmSubcommandDispatches) {
  testing::internal::CaptureStdout();
  int result = GhCommand{}.Run({"sm"});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}
