#include "ChooseAction.hpp"

#include "commands/ClaudeCLI/ChooseAction.hpp"
#include "commands/ClaudeCLI/Entry.hpp"
#include "commands/ClaudeDesk/Installation.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <csignal>
#include <filesystem>
#include <functional>
#include <set>

// ── data ──────────────────────────────────────────────────────────────────────

std::vector<CombinedProfile> FindCombinedProfiles() {
  const auto deskInstalls = FindVersionedInstallations();
  const auto cliGroups    = FindEntryGroups();

  std::set<std::string> deskSuffixes, cliSuffixes;
  for (const auto &i : deskInstalls) {
    const auto p = i.name.rfind('-');
    if (p != std::string::npos) deskSuffixes.insert(i.name.substr(p + 1));
  }
  for (const auto &g : cliGroups)
    if (!g.suffix.empty()) cliSuffixes.insert(g.suffix);

  std::set<std::string> all;
  all.insert(deskSuffixes.begin(), deskSuffixes.end());
  all.insert(cliSuffixes.begin(),  cliSuffixes.end());

  std::vector<CombinedProfile> result;
  for (const auto &s : all)
    result.push_back({s, deskSuffixes.count(s) > 0, cliSuffixes.count(s) > 0});
  return result;
}

// ── applier ───────────────────────────────────────────────────────────────────

void DefaultCombinedChooseApplier(const std::string &suffix) {
  const char *home = std::getenv("HOME");
  if (!home) return;

  std::error_code ec;
  const auto deskTarget =
      std::filesystem::path(home) / "Library" / "Application Support" /
      ("Claude-" + suffix);
  if (std::filesystem::exists(deskTarget, ec))
    SetActiveInstallation("Claude-" + suffix);

  const auto cliTarget =
      std::filesystem::path(home) / (".claude-" + suffix);
  if (std::filesystem::exists(cliTarget, ec) ||
      std::filesystem::is_symlink(cliTarget, ec))
    DefaultCLIChooseApplier(suffix);
}

// ── selector ──────────────────────────────────────────────────────────────────

static std::atomic<bool>     s_aborted{false};
static std::function<void()> s_exit_fn;

int DefaultCombinedChooseSelector(std::vector<std::string> &labels) {
  auto GetLinkTarget = [](const std::filesystem::path &path) -> std::string {
    std::error_code ec;
    if (std::filesystem::is_symlink(path, ec))
      return std::filesystem::read_symlink(path, ec).string();
    if (std::filesystem::exists(path, ec)) return "(not a symlink)";
    return "(not found)";
  };

  const char *home_cstr = std::getenv("HOME");
  std::string deskTarget, claudeTarget, claudeJsonTarget;
  if (home_cstr) {
    const std::filesystem::path home(home_cstr);
    deskTarget      = GetLinkTarget(home / "Library" / "Application Support" / "Claude");
    claudeTarget    = GetLinkTarget(home / ".claude");
    claudeJsonTarget = GetLinkTarget(home / ".claude.json");
  }

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
      ftxui::text("Claude         → " + deskTarget),
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

// ── entry point ───────────────────────────────────────────────────────────────

int RunCombinedChoose() { return CombinedChooseAction{}.Run(); }
