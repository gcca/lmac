#pragma once

#include "Entry.hpp"

#include <concepts>

class ClaudeCLIListAction {
public:
  template <class Finder>
    requires requires(Finder f) {
      { f() } -> std::convertible_to<std::vector<EntryGroup>>;
    }
  explicit ClaudeCLIListAction(Finder &&finder)
      : groups(std::forward<Finder>(finder)()) {}

  ClaudeCLIListAction() : groups(FindEntryGroups()) {}

  int Run();

private:
  std::vector<EntryGroup> groups;
};
