#include "ListAction.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>

static std::string GetLinkTarget(const std::filesystem::path &path) {
  std::error_code ec;
  if (std::filesystem::is_symlink(path, ec))
    return std::filesystem::read_symlink(path, ec).string();
  if (std::filesystem::exists(path, ec))
    return "(not a symlink)";
  return "(not found)";
}

int ClaudeCLIListAction::Run() {
  const char *home_cstr = std::getenv("HOME");
  if (!home_cstr) {
    std::cout << "HOME not set.\n";
    return 1;
  }
  const std::filesystem::path home(home_cstr);

  std::cout << "~/.claude      → " << GetLinkTarget(home / ".claude") << "\n"
            << "~/.claude.json → " << GetLinkTarget(home / ".claude.json") << "\n\n";

  const auto activeSuffix = GetActiveCLISuffix();

  if (groups.empty()) {
    std::cout << "No CLI profiles found.\n";
    return 0;
  }

  for (const auto &g : groups) {
    if (g.suffix.empty()) continue;
    const bool active = g.suffix == activeSuffix;
    std::cout << (active ? "▶ " : "  ")
              << g.suffix << "   "
              << ".claude-" << g.suffix << "   "
              << ".claude.json-" << g.suffix << "\n";
  }
  return 0;
}
