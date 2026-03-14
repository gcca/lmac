#include "GhCommand.hpp"

#include "Gh/SearchCommand.hpp"
#include "Gh/SmCommand.hpp"

#include <iostream>

static void PrintUsage() {
  std::cout << "Usage: lmac gh <subcommand>\n\n"
            << "Subcommands:\n"
            << "  help     show this help\n"
            << "  search   search pull requests or issues by owner\n"
            << "  sm       summarize pull requests or issues\n";
}

int GhCommand::Run(const std::vector<std::string> &args) {
  if (args.empty() || args[0] == "help") {
    PrintUsage();
    return 0;
  }

  const std::string sub = args[0];
  const std::vector<std::string> rest(args.begin() + 1, args.end());

  if (sub == "search")
    return SearchCommand{}.Run(rest);
  if (sub == "sm")
    return SmCommand{}.Run(rest);

  std::cerr << "Unknown subcommand: " << sub << "\n";
  return 1;
}
