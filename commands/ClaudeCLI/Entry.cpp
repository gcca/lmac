#include "Entry.hpp"

#include <algorithm>
#include <filesystem>
#include <map>
#include <stdexcept>

static std::filesystem::path HomePath() {
  const char *home = std::getenv("HOME");
  if (!home)
    throw std::runtime_error("HOME not set");
  return std::filesystem::path(home);
}

std::vector<EntryGroup> FindEntryGroups() {
  const auto home = HomePath();

  std::map<std::string, std::vector<std::string>> byKey;

  for (const auto *base : {".claude", ".claude.json"}) {
    const auto p = home / base;
    if (std::filesystem::exists(p) || std::filesystem::is_symlink(p))
      byKey[""].push_back(base);
  }

  for (const auto &item : std::filesystem::directory_iterator(home)) {
    const auto name = item.path().filename().string();
    if (!name.starts_with(".claude"))
      continue;
    if (name == ".claude" || name == ".claude.json")
      continue;
    const auto pos = name.rfind('-');
    if (pos == std::string::npos)
      continue;
    const auto suffix = name.substr(pos + 1);
    if (suffix.empty())
      continue;
    byKey[suffix].push_back(name);
  }

  for (auto &[key, names] : byKey)
    std::ranges::sort(names);

  std::vector<EntryGroup> result;
  if (auto it = byKey.find(""); it != byKey.end())
    result.push_back({"", it->second});
  for (auto &[suffix, names] : byKey) {
    if (suffix.empty())
      continue;
    result.push_back({suffix, std::move(names)});
  }

  return result;
}

std::string GetActiveCLISuffix() {
  const char *home = std::getenv("HOME");
  if (!home) return "";
  const auto link = std::filesystem::path(home) / ".claude";
  std::error_code ec;
  if (!std::filesystem::is_symlink(link, ec)) return "";
  const auto t = std::filesystem::read_symlink(link, ec).filename().string();
  const auto p = t.rfind('-');
  return p != std::string::npos ? t.substr(p + 1) : "";
}
