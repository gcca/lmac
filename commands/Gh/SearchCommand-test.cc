#include <gtest/gtest.h>

#include "SearchCommand.hpp"

TEST(SearchCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = SearchCommand{}.Run({});
  testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
}

TEST(SearchCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = SearchCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("search"), std::string::npos);
}

TEST(SearchCommandTest, MissingOwnerReturnsError) {
  testing::internal::CaptureStderr();
  int result = SearchCommand{}.Run({"prs"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(SearchCommandTest, MissingSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = SearchCommand{}.Run({"--owner", "my-org"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(SearchCommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = SearchCommand{}.Run({"unknown", "--owner", "my-org"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
