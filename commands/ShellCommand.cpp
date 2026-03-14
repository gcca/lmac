#include "ShellCommand.hpp"

#include "Shell/FormatAction.hpp"

#include <boost/program_options.hpp>
#include <iostream>

static void PrintUsage(const boost::program_options::options_description &desc) {
  std::cout << "Usage: lmac shell [options]\n\n" << desc << "\n";
}

static boost::program_options::options_description Options() {
  boost::program_options::options_description desc("Options");
  // clang-format off
  desc.add_options()
    ("format,f", boost::program_options::value<std::string>(), "shell format: fish, zsh, bash");
  // clang-format on
  return desc;
}

int ShellCommand::Run(const std::vector<std::string> &args) {
  auto desc = Options();

  if (args.empty() || args[0] == "help") {
    PrintUsage(desc);
    return 0;
  }

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::command_line_parser(args).options(desc).run(), vm);
  boost::program_options::notify(vm);

  if (vm.count("format"))
    return FormatAction{vm["format"].as<std::string>()}.Run();

  PrintUsage(desc);
  return 0;
}
