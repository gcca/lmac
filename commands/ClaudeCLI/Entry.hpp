#pragma once

#include <string>
#include <vector>

struct EntryGroup {
  std::string suffix;  // empty for base entries (.claude, .claude.json)
  std::vector<std::string> names;
};

std::vector<EntryGroup> FindEntryGroups();
std::string             GetActiveCLISuffix();
