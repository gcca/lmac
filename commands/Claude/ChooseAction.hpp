#pragma once

#include <concepts>
#include <iostream>
#include <string>
#include <vector>

struct CombinedProfile {
  std::string suffix;
  bool hasDeskInstall;
  bool hasCLIGroup;
};

std::vector<CombinedProfile> FindCombinedProfiles();

int DefaultCombinedChooseSelector(std::vector<std::string> &labels);
void DefaultCombinedChooseApplier(const std::string &suffix);

template <class Selector = decltype(&DefaultCombinedChooseSelector),
          class Applier  = decltype(&DefaultCombinedChooseApplier)>
  requires requires(Selector s, std::vector<std::string> &l) {
    { s(l) } -> std::convertible_to<int>;
  } && requires(Applier a, const std::string &suf) { a(suf); }
class CombinedChooseAction {
public:
  template <class Finder>
    requires requires(Finder f) {
      { f() } -> std::convertible_to<std::vector<CombinedProfile>>;
    }
  CombinedChooseAction(Finder &&finder,
                       Selector sel = DefaultCombinedChooseSelector,
                       Applier  app = DefaultCombinedChooseApplier)
      : profiles(std::forward<Finder>(finder)()),
        selector(std::move(sel)),
        applier(std::move(app)) {}

  CombinedChooseAction()
      : profiles(FindCombinedProfiles()),
        selector(DefaultCombinedChooseSelector),
        applier(DefaultCombinedChooseApplier) {}

  int Run() {
    if (profiles.empty()) {
      std::cerr << "No profiles found.\n";
      return 1;
    }

    size_t wSuffix = 0, wDesk = 0;
    for (const auto &p : profiles) {
      wSuffix = std::max(wSuffix, p.suffix.size());
      wDesk   = std::max(wDesk, DeskLabel(p).size());
    }

    std::vector<std::string> labels;
    for (const auto &p : profiles)
      labels.push_back(MakeLabel(p, wSuffix, wDesk));

    const int idx = selector(labels);
    if (idx < 0) return 0;

    try {
      applier(profiles[idx].suffix);
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
    return 0;
  }

private:
  std::vector<CombinedProfile> profiles;
  Selector selector;
  Applier  applier;

  static std::string DeskLabel(const CombinedProfile &p) {
    return p.hasDeskInstall ? "Claude-" + p.suffix : "(no desktop)";
  }

  static std::string CLILabel(const CombinedProfile &p) {
    return p.hasCLIGroup ? ".claude-" + p.suffix : "(no cli)";
  }

  static std::string MakeLabel(const CombinedProfile &p,
                               size_t wSuffix, size_t wDesk) {
    auto pad = [](const std::string &s, size_t w) -> std::string {
      return s + std::string(s.size() < w ? w - s.size() : 0, ' ');
    };
    return pad(p.suffix, wSuffix) + "   " +
           pad(DeskLabel(p), wDesk) + "   " +
           CLILabel(p);
  }
};

template <class Finder, class Selector>
CombinedChooseAction(Finder, Selector) -> CombinedChooseAction<Selector>;

template <class Finder, class Selector, class Applier>
CombinedChooseAction(Finder, Selector, Applier)
    -> CombinedChooseAction<Selector, Applier>;
