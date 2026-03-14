#include "commands/ClaudeCLICommand.hpp"
#include "commands/ClaudeCommand.hpp"
#include "commands/ClaudeDeskCommand.hpp"
#include "commands/GhCommand.hpp"
#include "commands/HelpCommand.hpp"
#include "commands/ShellCommand.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2)
    return HelpCommand{}.Run({});

  const std::string cmd = argv[1];
  const std::vector<std::string> args(argv + 2, argv + argc);

  if (cmd == "claude")
    return ClaudeCommand{}.Run(args);
  if (cmd == "claude-cli")
    return ClaudeCLICommand{}.Run(args);
  if (cmd == "claude-desk")
    return ClaudeDeskCommand{}.Run(args);
  if (cmd == "gh")
    return GhCommand{}.Run(args);
  if (cmd == "help")
    return HelpCommand{}.Run(args);
  if (cmd == "shell")
    return ShellCommand{}.Run(args);

  std::cerr << "Unknown command: " << cmd << "\n";
  return 1;
}
