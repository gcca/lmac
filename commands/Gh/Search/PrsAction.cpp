#include "PrsAction.hpp"

#include <cstdlib>

int DefaultPrsExecutor(const std::string &cmd) {
  return std::system(cmd.c_str());
}
