#include <gtest/gtest.h>

#include "HelpCommand.hpp"

TEST(HelpCommandTest, NoArgsShowsOverview) {
  testing::internal::CaptureStdout();
  int result = HelpCommand{}.Run({});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("claude-desk"), std::string::npos);
  EXPECT_NE(out.find("gh"), std::string::npos);
  EXPECT_NE(out.find("shell"), std::string::npos);
}

TEST(HelpCommandTest, GhArgShowsGhHelp) {
  testing::internal::CaptureStdout();
  int result = HelpCommand{}.Run({"gh"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("search"), std::string::npos);
  EXPECT_NE(out.find("sm"), std::string::npos);
}

TEST(HelpCommandTest, GhSmArgShowsSmHelp) {
  testing::internal::CaptureStdout();
  int result = HelpCommand{}.Run({"gh", "sm"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("pr"), std::string::npos);
  EXPECT_NE(out.find("issue"), std::string::npos);
}

TEST(HelpCommandTest, UnknownCommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = HelpCommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
