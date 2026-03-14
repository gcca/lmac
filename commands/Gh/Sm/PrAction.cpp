#include "PrAction.hpp"

#include <cstdlib>

int DefaultPrExecutor(const std::string &cmd) {
  return std::system(cmd.c_str());
}
