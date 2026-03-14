#include "Installation.hpp"

#include <filesystem>
#include <stdexcept>

static std::filesystem::path AppSupportPath() {
  const char *home = std::getenv("HOME");
  if (!home)
    throw std::runtime_error("HOME not set");
  return std::filesystem::path(home) / "Library" / "Application Support";
}

static std::string LinkLabel(const std::filesystem::path &path) {
  const auto target = std::filesystem::read_symlink(path).filename().string();
  return path.filename().string() + " (" + target + ")";
}

std::vector<Installation> FindInstallations() {
  std::vector<Installation> result;
  for (const auto &entry :
       std::filesystem::directory_iterator(AppSupportPath())) {
    const auto name = entry.path().filename().string();
    if (name != "Claude" && !name.starts_with("Claude-"))
      continue;
    const auto label = std::filesystem::is_symlink(entry.path())
                           ? LinkLabel(entry.path())
                           : name;
    result.push_back({name, label});
  }
  return result;
}

std::vector<Installation> FindVersionedInstallations() {
  std::vector<Installation> result;
  for (const auto &entry :
       std::filesystem::directory_iterator(AppSupportPath())) {
    const auto name = entry.path().filename().string();
    if (!name.starts_with("Claude-"))
      continue;
    result.push_back({name, name});
  }
  return result;
}

void SetActiveInstallation(const std::string &name) {
  const auto link = AppSupportPath() / "Claude";
  if (std::filesystem::is_symlink(link))
    std::filesystem::remove(link);
  std::filesystem::create_symlink(name, link);
}

std::string GetActiveDeskSuffix() {
  const char *home = std::getenv("HOME");
  if (!home) return "";
  const auto link = std::filesystem::path(home) / "Library" / "Application Support" / "Claude";
  std::error_code ec;
  if (!std::filesystem::is_symlink(link, ec)) return "";
  const auto t = std::filesystem::read_symlink(link, ec).filename().string();
  const auto p = t.rfind('-');
  return p != std::string::npos ? t.substr(p + 1) : "";
}
