#pragma once

#include "Installation.hpp"

#include <concepts>
#include <iostream>
#include <vector>

int DefaultChooseSelector(std::vector<std::string> &labels);
void DefaultChooseApplier(const std::string &name);

template <class Selector = decltype(&DefaultChooseSelector),
          class Applier = decltype(&DefaultChooseApplier)>
  requires requires(Selector s, std::vector<std::string> &labels) {
    { s(labels) } -> std::convertible_to<int>;
  } && requires(Applier a, const std::string &name) { a(name); }
class ChooseAction {
public:
  template <class Finder>
    requires requires(Finder f) {
      { f() } -> std::convertible_to<std::vector<Installation>>;
    }
  ChooseAction(Finder &&finder, Selector sel = DefaultChooseSelector,
               Applier app = DefaultChooseApplier)
      : installations(std::forward<Finder>(finder)()),
        selector(std::move(sel)),
        applier(std::move(app)) {}

  ChooseAction()
      : installations(FindVersionedInstallations()),
        selector(DefaultChooseSelector),
        applier(DefaultChooseApplier) {}

  int Run() {
    if (installations.empty()) {
      std::cerr << "No Claude installations found.\n";
      return 1;
    }
    std::vector<std::string> labels;
    for (const auto &i : installations)
      labels.push_back(i.label);
    const int idx = selector(labels);
    if (idx < 0) return 0;
    applier(installations[idx].name);
    return 0;
  }

private:
  std::vector<Installation> installations;
  Selector selector;
  Applier applier;
};

template <class Finder, class Selector>
ChooseAction(Finder, Selector) -> ChooseAction<Selector>;

template <class Finder, class Selector, class Applier>
ChooseAction(Finder, Selector, Applier) -> ChooseAction<Selector, Applier>;
