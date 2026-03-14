#pragma once

#include <string>
#include <vector>

class ShellCommand {
public:
  int Run(const std::vector<std::string> &args);
};
