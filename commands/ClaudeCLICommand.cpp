#include "ClaudeCLICommand.hpp"

#include "ClaudeCLI/ChooseAction.hpp"
#include "ClaudeCLI/ListAction.hpp"
#include "ClaudeCLI/ShowCredsAction.hpp"

#include <iostream>

static void PrintUsage() {
  std::cout << "Usage: lmac claude-cli <subcommand>\n\n"
            << "Subcommands:\n"
            << "  choose      interactively select a suffix group\n"
            << "  help        show this help\n"
            << "  list        list Claude CLI configs and data directories\n"
            << "  show-creds  print Claude Code keychain credentials\n";
}

int ClaudeCLICommand::Run(const std::vector<std::string> &args) {
  if (args.empty() || args[0] == "help") {
    PrintUsage();
    return 0;
  }

  const std::string sub = args[0];

  if (sub == "choose")
    return ClaudeCLIChooseAction{}.Run();
  if (sub == "list")
    return ClaudeCLIListAction{}.Run();
  if (sub == "show-creds")
    return ShowCredsAction{}.Run();

  std::cerr << "Unknown subcommand: " << sub << "\n";
  return 1;
}
