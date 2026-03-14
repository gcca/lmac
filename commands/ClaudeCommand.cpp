#include "ClaudeCommand.hpp"

#include "ClaudeCLI/Entry.hpp"
#include "ClaudeDesk/Installation.hpp"

#include <algorithm>
#include <iostream>
#include <set>

int RunCombinedList() {
  const auto deskActive = GetActiveDeskSuffix();
  const auto cliActive  = GetActiveCLISuffix();

  const auto deskInstalls = FindVersionedInstallations();
  const auto cliGroups    = FindEntryGroups();

  std::set<std::string> suffixes;
  for (const auto &i : deskInstalls) {
    const auto p = i.name.rfind('-');
    if (p != std::string::npos) suffixes.insert(i.name.substr(p + 1));
  }
  for (const auto &g : cliGroups)
    if (!g.suffix.empty()) suffixes.insert(g.suffix);

  if (suffixes.empty()) {
    std::cout << "No profiles found.\n";
    return 0;
  }

  // Compute column widths
  const std::string hProfile = "profile";
  const std::string hDesktop = "desktop";
  const std::string hCLI     = "cli";

  size_t wProfile = hProfile.size();
  size_t wDesktop = hDesktop.size();
  size_t wCLI     = hCLI.size();
  for (const auto &s : suffixes) {
    wProfile = std::max(wProfile, s.size());
    wDesktop = std::max(wDesktop, 7 + s.size());   // "Claude-X"
    wCLI     = std::max(wCLI,     8 + s.size());   // ".claude-X"
  }

  auto pad = [](const std::string &s, size_t w) -> std::string {
    return s + std::string(s.size() < w ? w - s.size() : 0, ' ');
  };

  // Header
  std::cout << "  " << pad(hProfile, wProfile) << "  "
            << pad(hDesktop, wDesktop) << "  " << hCLI << "\n";
  std::cout << "  " << std::string(wProfile, '-') << "  "
            << std::string(wDesktop, '-') << "  "
            << std::string(wCLI, '-') << "\n";

  // Rows
  for (const auto &s : suffixes) {
    const bool active = (s == deskActive || s == cliActive);
    std::cout << (active ? "▶ " : "  ")
              << pad(s, wProfile) << "  "
              << pad("Claude-" + s, wDesktop) << "  "
              << ".claude-" << s << "\n";
  }

  return 0;
}
