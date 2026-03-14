#pragma once

#include <string>
#include <vector>

class GhCommand {
public:
  int Run(const std::vector<std::string> &args);
};
