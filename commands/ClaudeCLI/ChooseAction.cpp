#include "ChooseAction.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <stdexcept>

static std::filesystem::path HomePath() {
  const char *home = std::getenv("HOME");
  if (!home)
    throw std::runtime_error("HOME not set");
  return std::filesystem::path(home);
}

static void ReplaceSymlink(const std::filesystem::path &link,
                           const std::string &target) {
  if (std::filesystem::is_symlink(link) || std::filesystem::exists(link))
    std::filesystem::remove(link);
  std::filesystem::create_symlink(target, link);
}

static void UpdateKeychainCredential(const std::string &password) {
  static constexpr const char *kService = "Claude Code-credentials";
  const char *user = std::getenv("USER");
  if (!user) user = "";

  auto StripTrailingNewlines = [](std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r'))
      s.pop_back();
    return s;
  };

  auto DeleteAllMatchingKeychainItems = [](const std::string &svc) {
    while (std::system(("security delete-generic-password"
                        " -s '" + svc + "' 2>/dev/null").c_str()) == 0) {}
  };

  const std::string pw  = StripTrailingNewlines(password);
  const std::string svc = kService;

  DeleteAllMatchingKeychainItems(svc);

  std::system(("security add-generic-password"
               " -s '" + svc + "'"
               " -a '" + std::string(user) + "'"
               " -A"
               " -w '" + pw + "'").c_str());
}

void DefaultCLIChooseApplier(const std::string &suffix) {
  const auto home = HomePath();

  ReplaceSymlink(home / ".claude",      ".claude-"      + suffix);
  ReplaceSymlink(home / ".claude.json", ".claude.json-" + suffix);

  const auto credsPath = home / (".claude-creds-" + suffix);
  std::ifstream file(credsPath);
  if (!file)
    throw std::runtime_error("Cannot read " + credsPath.string());
  const std::string content(std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>{});

  UpdateKeychainCredential(content);
}

// SIGINT support: globals safe to use from signal handler
static std::atomic<bool>        s_aborted{false};
static std::function<void()>    s_exit_fn;

int DefaultCLIChooseSelector(std::vector<std::string> &labels) {
  auto GetLinkTarget = [](const std::filesystem::path &path) -> std::string {
    std::error_code ec;
    if (std::filesystem::is_symlink(path, ec))
      return std::filesystem::read_symlink(path, ec).string();
    if (std::filesystem::exists(path, ec))
      return "(not a symlink)";
    return "(not found)";
  };

  const auto home = HomePath();
  const std::string claudeTarget     = GetLinkTarget(home / ".claude");
  const std::string claudeJsonTarget = GetLinkTarget(home / ".claude.json");

  int selected = 0;
  s_aborted = false;
  auto screen = ftxui::ScreenInteractive::FitComponent();
  s_exit_fn = screen.ExitLoopClosure();

  auto prev_handler = std::signal(SIGINT, [](int) {
    s_aborted = true;
    s_exit_fn();
  });

  auto menu = ftxui::Menu(&labels, &selected);

  auto renderer = ftxui::Renderer(menu, [&] {
    return ftxui::vbox({
      ftxui::text("~/.claude      → " + claudeTarget),
      ftxui::text("~/.claude.json → " + claudeJsonTarget),
      ftxui::separator(),
      menu->Render(),
    });
  });

  auto component = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
    if (event == ftxui::Event::Return) {
      screen.ExitLoopClosure()();
      return true;
    }
    // Fallback: if FTXUI disables ISIG, Ctrl-C arrives as \x03
    if (event == ftxui::Event::Character('\x03') ||
        event == ftxui::Event::Escape) {
      s_aborted = true;
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(component);
  std::signal(SIGINT, prev_handler);
  s_exit_fn = nullptr;

  if (s_aborted) return -1;
  return selected;
}
