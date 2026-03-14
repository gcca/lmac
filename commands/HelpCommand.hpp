#pragma once

#include <string>
#include <vector>

class HelpCommand {
public:
  int Run(const std::vector<std::string> &args);
};
