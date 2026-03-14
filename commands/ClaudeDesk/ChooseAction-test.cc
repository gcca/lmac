#include <gtest/gtest.h>

#include "ChooseAction.hpp"

TEST(ChooseActionTest, EmptyInstallationsReturnsError) {
  testing::internal::CaptureStderr();
  int result = ChooseAction([]() { return std::vector<Installation>{}; }).Run();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
}

TEST(ChooseActionTest, SelectorReceivesLabels) {
  int callCount = 0;
  auto finder = []() {
    return std::vector<Installation>{{"Claude-PS", "Claude-PS"},
                                     {"Claude-GC", "Claude-GC"}};
  };
  auto selector = [&callCount](std::vector<std::string> &) -> int {
    ++callCount;
    return 0;
  };
  std::string applied;
  auto applier = [&applied](const std::string &name) { applied = name; };

  int result = ChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(applied, "Claude-PS");
}

TEST(ChooseActionTest, SelectorIndexDeterminesApplied) {
  auto finder = []() {
    return std::vector<Installation>{{"Claude-PS", "Claude-PS"},
                                     {"Claude-GC", "Claude-GC"}};
  };
  auto selector = [](std::vector<std::string> &) -> int { return 1; };
  std::string applied;
  auto applier = [&applied](const std::string &name) { applied = name; };

  int result = ChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(applied, "Claude-GC");
}

TEST(ChooseActionTest, ApplierCalledWithSelectedName) {
  auto finder = []() {
    return std::vector<Installation>{{"Claude-A", "Claude-A"},
                                     {"Claude-B", "Claude-B"},
                                     {"Claude-C", "Claude-C"}};
  };
  auto selector = [](std::vector<std::string> &) -> int { return 2; };
  std::string applied;
  auto applier = [&applied](const std::string &name) { applied = name; };

  ChooseAction(finder, selector, applier).Run();

  EXPECT_EQ(applied, "Claude-C");
}
