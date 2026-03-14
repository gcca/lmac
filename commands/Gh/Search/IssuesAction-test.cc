#include <gtest/gtest.h>

#include "IssuesAction.hpp"

TEST(IssuesActionTest, ExecutorReceivesCommand) {
  using namespace std::chrono;
  DateRange range{
      .start = 2026y / January / 1d,
      .end = 2026y / April / 1d,
  };
  std::string captured;
  auto exec = [&captured](const std::string &cmd) -> int {
    captured = cmd;
    return 0;
  };

  int result = IssuesAction(range, "my-org", exec).Run();

  EXPECT_EQ(result, 0);
  EXPECT_NE(captured.find("gh search issues"), std::string::npos);
  EXPECT_NE(captured.find("--owner=my-org"), std::string::npos);
  EXPECT_NE(captured.find("2026-01-01..2026-04-01"), std::string::npos);
}
