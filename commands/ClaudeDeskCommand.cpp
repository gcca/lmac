#include "ClaudeDeskCommand.hpp"

#include "ClaudeDesk/ChooseAction.hpp"
#include "ClaudeDesk/ListAction.hpp"

#include <iostream>

static void PrintUsage() {
  std::cout << "Usage: lmac claude-desk <subcommand>\n\n"
            << "Subcommands:\n"
            << "  choose   interactively select a Claude installation\n"
            << "  help     show this help\n"
            << "  list     list all Claude installations\n";
}

int ClaudeDeskCommand::Run(const std::vector<std::string> &args) {
  if (args.empty() || args[0] == "help") {
    PrintUsage();
    return 0;
  }

  const std::string sub = args[0];

  if (sub == "choose")
    return ChooseAction{}.Run();
  if (sub == "list")
    return ListAction{}.Run();

  std::cerr << "Unknown subcommand: " << sub << "\n";
  return 1;
}
