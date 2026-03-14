#include "ChooseAction.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <csignal>
#include <filesystem>
#include <functional>

void DefaultChooseApplier(const std::string &name) {
  SetActiveInstallation(name);
}

static std::atomic<bool>     s_aborted{false};
static std::function<void()> s_exit_fn;

int DefaultChooseSelector(std::vector<std::string> &labels) {
  const char *home_cstr = std::getenv("HOME");
  std::string currentTarget = "(unknown)";
  if (home_cstr) {
    const std::filesystem::path link =
        std::filesystem::path(home_cstr) / "Library" / "Application Support" / "Claude";
    std::error_code ec;
    if (std::filesystem::is_symlink(link, ec))
      currentTarget = std::filesystem::read_symlink(link, ec).string();
    else if (std::filesystem::exists(link, ec))
      currentTarget = "(not a symlink)";
    else
      currentTarget = "(not found)";
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
      ftxui::text("Claude → " + currentTarget),
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
