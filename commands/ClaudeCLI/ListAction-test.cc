#include <gtest/gtest.h>

#include "ListAction.hpp"

TEST(ClaudeCLIListActionTest, EmptyGroupsPrintsNotFound) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLIListAction([]() { return std::vector<EntryGroup>{}; }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("No Claude CLI entries found"), std::string::npos);
}

TEST(ClaudeCLIListActionTest, BaseGroupPrintsWithoutHeader) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLIListAction([]() {
    return std::vector<EntryGroup>{{"", {".claude", ".claude.json"}}};
  }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("  .claude\n"), std::string::npos);
  EXPECT_NE(out.find("  .claude.json\n"), std::string::npos);
  EXPECT_EQ(out.find('['), std::string::npos);
}

TEST(ClaudeCLIListActionTest, SuffixGroupPrintsWithHeader) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLIListAction([]() {
    return std::vector<EntryGroup>{
        {"gc", {".claude-gc", ".claude-creds-gc", ".claude.json-gc"}}};
  }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("[gc]\n"), std::string::npos);
  EXPECT_NE(out.find("  .claude-gc\n"), std::string::npos);
  EXPECT_NE(out.find("  .claude-creds-gc\n"), std::string::npos);
  EXPECT_NE(out.find("  .claude.json-gc\n"), std::string::npos);
}

TEST(ClaudeCLIListActionTest, MultipleGroupsSeparatedByBlankLine) {
  testing::internal::CaptureStdout();
  int result = ClaudeCLIListAction([]() {
    return std::vector<EntryGroup>{
        {"gc", {".claude-gc", ".claude.json-gc"}},
        {"ps", {".claude-ps", ".claude.json-ps"}}};
  }).Run();
  std::string out = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_NE(out.find("[gc]\n"), std::string::npos);
  EXPECT_NE(out.find("[ps]\n"), std::string::npos);
  // blank line separates groups
  EXPECT_NE(out.find("\n\n"), std::string::npos);
}
