#include <gtest/gtest.h>

#include "ClaudeCommand.hpp"

TEST(ClaudeCommandTest, NoArgsShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeCommand{}.Run({});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("claude"), std::string::npos);
}

TEST(ClaudeCommandTest, HelpSubcommandShowsUsage) {
  testing::internal::CaptureStdout();
  int result = ClaudeCommand{}.Run({"help"});
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("list"), std::string::npos);
  EXPECT_NE(out.find("choose"), std::string::npos);
}

TEST(ClaudeCommandTest, UnknownSubcommandReturnsError) {
  testing::internal::CaptureStderr();
  int result = ClaudeCommand{}.Run({"unknown"});
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(ClaudeCommandTest, ListSubcommandCallsCombinedLister) {
  bool called = false;
  auto lister  = [&called]() -> int { called = true; return 0; };
  auto chooser = []() -> int { return 0; };

  int result = ClaudeCommand(lister, chooser).Run({"list"});

  EXPECT_EQ(result, 0);
  EXPECT_TRUE(called);
}

TEST(ClaudeCommandTest, ChooseSubcommandCallsCombinedChooser) {
  bool called = false;
  auto lister  = []() -> int { return 0; };
  auto chooser = [&called]() -> int { called = true; return 0; };

  int result = ClaudeCommand(lister, chooser).Run({"choose"});

  EXPECT_EQ(result, 0);
  EXPECT_TRUE(called);
}

TEST(ClaudeCommandTest, ListSubcommandPropagatesError) {
  auto lister  = []() -> int { return 42; };
  auto chooser = []() -> int { return 0; };
  EXPECT_EQ(ClaudeCommand(lister, chooser).Run({"list"}), 42);
}

TEST(ClaudeCommandTest, ChooseSubcommandPropagatesError) {
  auto lister  = []() -> int { return 0; };
  auto chooser = []() -> int { return 7; };
  EXPECT_EQ(ClaudeCommand(lister, chooser).Run({"choose"}), 7);
}
