#include <gtest/gtest.h>

#include "ListAction.hpp"

TEST(ListActionTest, EmptyInstallationsPrintsNotFound) {
  testing::internal::CaptureStdout();
  int result = ListAction([]() { return std::vector<Installation>{}; }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(out, "No Claude installations found.\n");
}

TEST(ListActionTest, InstallationPrintsLabel) {
  testing::internal::CaptureStdout();
  int result = ListAction([]() {
                 return std::vector<Installation>{{"Claude-PS", "Claude-PS"}};
               }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(out, "Claude-PS\n");
}

TEST(ListActionTest, SymlinkInstallationPrintsLabelWithTarget) {
  testing::internal::CaptureStdout();
  int result =
      ListAction([]() {
        return std::vector<Installation>{{"Claude", "Claude (Claude-PS)"}};
      }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(out, "Claude (Claude-PS)\n");
}

TEST(ListActionTest, MultipleInstallationsPrintsAll) {
  testing::internal::CaptureStdout();
  int result = ListAction([]() {
                 return std::vector<Installation>{
                     {"Claude", "Claude (Claude-PS)"},
                     {"Claude-PS", "Claude-PS"},
                     {"Claude-GC", "Claude-GC"},
                 };
               }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(out, "Claude (Claude-PS)\nClaude-PS\nClaude-GC\n");
}
