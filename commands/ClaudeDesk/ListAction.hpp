#pragma once

#include "Installation.hpp"

#include <concepts>

class ListAction {
public:
  template <class Finder>
    requires requires(Finder f) {
      { f() } -> std::convertible_to<std::vector<Installation>>;
    }
  explicit ListAction(Finder &&finder)
      : installations(std::forward<Finder>(finder)()) {}

  ListAction() : installations(FindInstallations()) {}

  int Run();

private:
  std::vector<Installation> installations;
};
