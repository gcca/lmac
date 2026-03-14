#include "ListAction.hpp"

#include <iostream>

int ListAction::Run() {
  const auto activeSuffix = GetActiveDeskSuffix();

  if (!activeSuffix.empty())
    std::cout << "Claude → Claude-" << activeSuffix << "\n\n";

  bool any = false;
  for (const auto &i : installations) {
    if (i.name == "Claude") continue;
    any = true;
    const auto p       = i.name.rfind('-');
    const auto suffix  = p != std::string::npos ? i.name.substr(p + 1) : "";
    const bool active  = !suffix.empty() && suffix == activeSuffix;
    std::cout << (active ? "▶ " : "  ") << i.name << "\n";
  }
  if (!any)
    std::cout << "No Claude installations found.\n";
  return 0;
}
