#pragma once

#include "Entry.hpp"

#include <concepts>
#include <iostream>
#include <numeric>

int DefaultCLIChooseSelector(std::vector<std::string> &labels);
void DefaultCLIChooseApplier(const std::string &suffix);

template <class Selector = decltype(&DefaultCLIChooseSelector),
          class Applier  = decltype(&DefaultCLIChooseApplier)>
  requires requires(Selector s, std::vector<std::string> &labels) {
    { s(labels) } -> std::convertible_to<int>;
  } && requires(Applier a, const std::string &suffix) { a(suffix); }
class ClaudeCLIChooseAction {
public:
  template <class Finder>
    requires requires(Finder f) {
      { f() } -> std::convertible_to<std::vector<EntryGroup>>;
    }
  ClaudeCLIChooseAction(Finder &&finder, Selector sel = DefaultCLIChooseSelector,
                        Applier app = DefaultCLIChooseApplier)
      : groups(SuffixGroups(std::forward<Finder>(finder)())),
        selector(std::move(sel)),
        applier(std::move(app)) {}

  ClaudeCLIChooseAction()
      : groups(SuffixGroups(FindEntryGroups())),
        selector(DefaultCLIChooseSelector),
        applier(DefaultCLIChooseApplier) {}

  int Run() {
    if (groups.empty()) {
      std::cerr << "No Claude CLI suffix groups found.\n";
      return 1;
    }
    std::vector<std::string> labels;
    for (const auto &g : groups)
      labels.push_back(MakeLabel(g));
    const int idx = selector(labels);
    if (idx < 0) return 0;
    try {
      applier(groups[idx].suffix);
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
    return 0;
  }

private:
  std::vector<EntryGroup> groups;
  Selector selector;
  Applier applier;

  static std::vector<EntryGroup> SuffixGroups(std::vector<EntryGroup> all) {
    std::vector<EntryGroup> result;
    for (auto &g : all)
      if (!g.suffix.empty())
        result.push_back(std::move(g));
    return result;
  }

  static std::string MakeLabel(const EntryGroup &g) {
    return g.suffix + "  (" +
           std::accumulate(
               std::next(g.names.begin()), g.names.end(), g.names.front(),
               [](const std::string &a, const std::string &b) {
                 return a + ", " + b;
               }) +
           ")";
  }
};

template <class Finder, class Selector>
ClaudeCLIChooseAction(Finder, Selector) -> ClaudeCLIChooseAction<Selector>;

template <class Finder, class Selector, class Applier>
ClaudeCLIChooseAction(Finder, Selector, Applier)
    -> ClaudeCLIChooseAction<Selector, Applier>;
