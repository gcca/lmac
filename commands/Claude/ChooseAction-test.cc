#include <gtest/gtest.h>

#include "commands/Claude/ChooseAction.hpp"

TEST(CombinedChooseActionTest, EmptyProfilesReturnsError) {
  auto finder   = []() { return std::vector<CombinedProfile>{}; };
  auto selector = [](std::vector<std::string> &) -> int { return 0; };
  auto applier  = [](const std::string &) {};

  testing::internal::CaptureStderr();
  int result = CombinedChooseAction(finder, selector, applier).Run();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(CombinedChooseActionTest, SelectorReceivesFormattedLabels) {
  std::vector<CombinedProfile> profiles = {
      {"gc", true, true},
      {"ps", true, false},
      {"rm", false, true},
  };
  auto finder = [&profiles]() { return profiles; };

  std::vector<std::string> capturedLabels;
  auto selector = [&capturedLabels](std::vector<std::string> &labels) -> int {
    capturedLabels = labels;
    return 0;
  };
  auto applier = [](const std::string &) {};

  CombinedChooseAction(finder, selector, applier).Run();

  ASSERT_EQ(capturedLabels.size(), 3u);
  EXPECT_NE(capturedLabels[0].find("Claude-gc"), std::string::npos);
  EXPECT_NE(capturedLabels[0].find(".claude-gc"), std::string::npos);
  EXPECT_NE(capturedLabels[1].find("Claude-ps"), std::string::npos);
  EXPECT_NE(capturedLabels[1].find("(no cli)"), std::string::npos);
  EXPECT_NE(capturedLabels[2].find("(no desktop)"), std::string::npos);
  EXPECT_NE(capturedLabels[2].find(".claude-rm"), std::string::npos);
}

TEST(CombinedChooseActionTest, SelectorIndexDeterminesAppliedSuffix) {
  std::vector<CombinedProfile> profiles = {
      {"gc", true, true},
      {"ps", true, true},
  };
  auto finder = [&profiles]() { return profiles; };
  auto selector = [](std::vector<std::string> &) -> int { return 1; };

  std::string applied;
  auto applier = [&applied](const std::string &suf) { applied = suf; };

  CombinedChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(applied, "ps");
}

TEST(CombinedChooseActionTest, NegativeSelectorIndexAbortsWithoutApplying) {
  std::vector<CombinedProfile> profiles = {{"gc", true, true}};
  auto finder   = [&profiles]() { return profiles; };
  auto selector = [](std::vector<std::string> &) -> int { return -1; };

  bool applied = false;
  auto applier = [&applied](const std::string &) { applied = true; };

  int result = CombinedChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(result, 0);
  EXPECT_FALSE(applied);
}

TEST(CombinedChooseActionTest, ApplierExceptionReturnsError) {
  std::vector<CombinedProfile> profiles = {{"gc", true, true}};
  auto finder   = [&profiles]() { return profiles; };
  auto selector = [](std::vector<std::string> &) -> int { return 0; };
  auto applier  = [](const std::string &) {
    throw std::runtime_error("fail");
  };

  testing::internal::CaptureStderr();
  int result = CombinedChooseAction(finder, selector, applier).Run();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}
