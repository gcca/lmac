#pragma once

#include <concepts>
#include <iostream>
#include <string>
#include <vector>

int RunCombinedList();
int RunCombinedChoose();

template <class Lister  = decltype(&RunCombinedList),
          class Chooser = decltype(&RunCombinedChoose)>
  requires requires(Lister  l) { { l() } -> std::convertible_to<int>; }
        && requires(Chooser c) { { c() } -> std::convertible_to<int>; }
class ClaudeCommand {
public:
  ClaudeCommand(Lister  lister  = RunCombinedList,
                Chooser chooser = RunCombinedChoose)
      : lister_(std::move(lister)), chooser_(std::move(chooser)) {}

  int Run(const std::vector<std::string> &args) {
    if (args.empty() || args[0] == "help") {
      PrintUsage();
      return 0;
    }
    if (args[0] == "list")   return lister_();
    if (args[0] == "choose") return chooser_();
    std::cerr << "Unknown subcommand: " << args[0] << "\n";
    return 1;
  }

private:
  Lister  lister_;
  Chooser chooser_;

  static void PrintUsage() {
    std::cout << "Usage: lmac claude <subcommand>\n\n"
              << "Subcommands:\n"
              << "  choose   interactively select a profile (updates Desktop + CLI symlinks and keychain)\n"
              << "  help     show this help\n"
              << "  list     combined list of Desktop and CLI profiles\n";
  }
};

template <class L, class C>
ClaudeCommand(L, C) -> ClaudeCommand<L, C>;
