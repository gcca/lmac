#include <gtest/gtest.h>

#include "ChooseAction.hpp"

TEST(ClaudeCLIChooseActionTest, NoSuffixGroupsReturnsError) {
  testing::internal::CaptureStderr();
  int result = ClaudeCLIChooseAction(
      []() { return std::vector<EntryGroup>{{"", {".claude", ".claude.json"}}}; }
  ).Run();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(ClaudeCLIChooseActionTest, SelectorReceivesLabelsWithFiles) {
  std::vector<std::string> receivedLabels;
  auto finder = []() {
    return std::vector<EntryGroup>{
        {"gc", {".claude-gc", ".claude-creds-gc", ".claude.json-gc"}},
        {"ps", {".claude-ps", ".claude.json-ps"}}};
  };
  auto selector = [&receivedLabels](std::vector<std::string> &labels) -> int {
    receivedLabels = labels;
    return 0;
  };
  std::string applied;
  auto applier = [&applied](const std::string &s) { applied = s; };

  int result = ClaudeCLIChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(result, 0);
  ASSERT_EQ(receivedLabels.size(), 2u);
  EXPECT_NE(receivedLabels[0].find("gc"), std::string::npos);
  EXPECT_NE(receivedLabels[0].find(".claude-gc"), std::string::npos);
  EXPECT_EQ(applied, "gc");
}

TEST(ClaudeCLIChooseActionTest, SelectorIndexDeterminesApplied) {
  auto finder = []() {
    return std::vector<EntryGroup>{
        {"gc", {".claude-gc"}},
        {"ps", {".claude-ps"}}};
  };
  auto selector = [](std::vector<std::string> &) -> int { return 1; };
  std::string applied;
  auto applier = [&applied](const std::string &s) { applied = s; };

  ClaudeCLIChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(applied, "ps");
}

TEST(ClaudeCLIChooseActionTest, ApplierExceptionReturnsError) {
  auto finder = []() {
    return std::vector<EntryGroup>{{"gc", {".claude-gc"}}};
  };
  auto selector = [](std::vector<std::string> &) -> int { return 0; };
  auto applier = [](const std::string &) {
    throw std::runtime_error("symlink failed");
  };

  testing::internal::CaptureStderr();
  int result = ClaudeCLIChooseAction(finder, selector, applier).Run();
  std::string err = testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
  EXPECT_NE(err.find("symlink failed"), std::string::npos);
}

TEST(ClaudeCLIChooseActionTest, BaseGroupIsExcludedFromMenu) {
  int selectorCallCount = 0;
  auto finder = []() {
    return std::vector<EntryGroup>{
        {"", {".claude", ".claude.json"}},
        {"gc", {".claude-gc"}}};
  };
  auto selector = [&selectorCallCount](std::vector<std::string> &labels) -> int {
    selectorCallCount++;
    EXPECT_EQ(labels.size(), 1u);  // only "gc", not the base group
    return 0;
  };
  auto applier = [](const std::string &) {};

  ClaudeCLIChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(selectorCallCount, 1);
}
