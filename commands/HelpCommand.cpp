#include "HelpCommand.hpp"

#include "ClaudeCLICommand.hpp"
#include "ClaudeCommand.hpp"
#include "ClaudeDeskCommand.hpp"
#include "GhCommand.hpp"
#include "ShellCommand.hpp"

#include <iostream>

static void PrintOverview() {
  std::cout <<
      "Usage: lmac <command> [subcommand] [options]\n"
      "\n"
      "Commands:\n"
      "  claude        combined Claude overview\n"
      "  claude-cli    manage Claude CLI configs and data directories\n"
      "  claude-desk   manage Claude desktop installations\n"
      "  gh            GitHub utilities\n"
      "  help          show this help or help for a specific command\n"
      "  shell         generate shell completions\n"
      "\n"
      "claude:\n"
      "  lmac claude list                     combined list of Desktop and CLI installations\n"
      "\n"
      "claude-cli:\n"
      "  lmac claude-cli list                 list ~/.claude, ~/.claude-*, ~/.claude.json, ~/.claude.json-*\n"
      "\n"
      "claude-desk:\n"
      "  lmac claude-desk list                list all Claude installations\n"
      "  lmac claude-desk choose              interactive selection menu\n"
      "\n"
      "gh search:\n"
      "  lmac gh search prs   --owner <org>   search PRs in an org/user\n"
      "  lmac gh search issues --owner <org>  search issues in an org/user\n"
      "    -d, --date YYYY-MM-DD              reference date (default: today)\n"
      "    -m, --months N                     months back for range start (default: 1)\n"
      "    -o, --owner <org|user>             GitHub org or user (required)\n"
      "\n"
      "gh sm:\n"
      "  lmac gh sm pr                        search your PRs\n"
      "  lmac gh sm issue                     search your issues\n"
      "    -d, --date YYYY-MM-DD              reference date (default: today)\n"
      "    -m, --months N                     months back for range start (default: 1)\n"
      "\n"
      "shell:\n"
      "  lmac shell --format fish             print fish completions to stdout\n"
      "\n"
      "For command-specific help:\n"
      "  lmac help <command>                  e.g. lmac help gh\n"
      "  lmac help <command> <subcommand>     e.g. lmac help gh sm\n"
      "  lmac <command> --help\n";
}

static std::vector<std::string> ArgsWithHelp(const std::vector<std::string> &args) {
  std::vector<std::string> result(args.begin() + 1, args.end());
  result.push_back("help");
  return result;
}

int HelpCommand::Run(const std::vector<std::string> &args) {
  if (args.empty()) {
    PrintOverview();
    return 0;
  }

  const std::string cmd = args[0];

  if (cmd == "claude")
    return ClaudeCommand{}.Run(ArgsWithHelp(args));
  if (cmd == "claude-cli")
    return ClaudeCLICommand{}.Run(ArgsWithHelp(args));
  if (cmd == "claude-desk")
    return ClaudeDeskCommand{}.Run(ArgsWithHelp(args));
  if (cmd == "gh")
    return GhCommand{}.Run(ArgsWithHelp(args));
  if (cmd == "shell")
    return ShellCommand{}.Run(ArgsWithHelp(args));

  std::cerr << "Unknown command: " << cmd << "\n";
  return 1;
}
