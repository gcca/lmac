#include "IssueAction.hpp"

#include <cstdlib>

int DefaultIssueExecutor(const std::string &cmd) {
  return std::system(cmd.c_str());
}
