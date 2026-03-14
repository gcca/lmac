#include "IssuesAction.hpp"

#include <cstdlib>

int DefaultIssuesExecutor(const std::string &cmd) {
  return std::system(cmd.c_str());
}
