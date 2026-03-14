#pragma once

#include <string>

class FormatAction {
public:
  FormatAction(std::string fmt);
  int Run();

private:
  std::string fmt;
  void PrintFishCompletions();
};
