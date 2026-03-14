#include "SmCommand.hpp"

#include "DateRange.hpp"
#include "Sm/IssueAction.hpp"
#include "Sm/PrAction.hpp"

#include <boost/program_options.hpp>

#include <chrono>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>

static std::chrono::year_month_day Today() {
  return std::chrono::year_month_day{
      std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

static std::string FormatDate(std::chrono::year_month_day ymd) {
  return std::format("{:%Y-%m-%d}", ymd);
}

static std::chrono::year_month_day ParseDate(const std::string &s) {
  std::tm tm{};
  std::istringstream ss(s);
  ss >> std::get_time(&tm, "%Y-%m-%d");
  return std::chrono::year_month_day{
      std::chrono::year{tm.tm_year + 1900},
      std::chrono::month{static_cast<unsigned>(tm.tm_mon + 1)},
      std::chrono::day{static_cast<unsigned>(tm.tm_mday)},
  };
}

static DateRange ComputeDateRange(std::chrono::year_month_day date,
                                  int months) {
  auto ym = date.year() / date.month();
  return {
      .start = (ym - std::chrono::months{months}) / std::chrono::day{1},
      .end = (ym + std::chrono::months{1}) / std::chrono::day{1},
  };
}

static void
PrintUsage(const boost::program_options::options_description &visible) {
  std::cout << "Usage: lmac gh sm <pr|issue|help> [options]\n\n"
            << "Arguments:\n"
            << "  help    show this help\n"
            << "  issue   search issues\n"
            << "  pr      search pull requests\n\n"
            << visible << "\n";
}

static boost::program_options::options_description VisibleOptions() {
  boost::program_options::options_description desc("Options");
  // clang-format off
  desc.add_options()
    ("date,d",   boost::program_options::value<std::string>()->default_value(FormatDate(Today())), "Date in YYYY-MM-DD format (default: today)")
    ("months,m", boost::program_options::value<int>()->default_value(1),                           "Months back for range start (default: 1)");
  // clang-format on
  return desc;
}

static boost::program_options::options_description HiddenOptions() {
  boost::program_options::options_description hidden;
  // clang-format off
  hidden.add_options()
    ("subcommand", boost::program_options::value<std::string>());
  // clang-format on
  return hidden;
}

int SmCommand::Run(const std::vector<std::string> &args) {
  auto visible = VisibleOptions();

  if (args.empty() || args[0] == "help") {
    PrintUsage(visible);
    return 0;
  }

  auto hidden = HiddenOptions();

  boost::program_options::options_description all;
  all.add(visible).add(hidden);

  boost::program_options::positional_options_description pos;
  pos.add("subcommand", 1);

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::command_line_parser(args)
          .options(all)
          .positional(pos)
          .run(),
      vm);
  boost::program_options::notify(vm);

  if (!vm.count("subcommand")) {
    std::cerr << "Expected subcommand: pr or issue\n";
    return 1;
  }

  auto date = ParseDate(vm["date"].as<std::string>());
  auto months = vm["months"].as<int>();
  auto range = ComputeDateRange(date, months);
  auto sub = vm["subcommand"].as<std::string>();

  if (sub == "pr")
    return PrAction{range}.Run();
  if (sub == "issue")
    return IssueAction{range}.Run();

  std::cerr << "Unknown subcommand: " << sub << "\n";
  return 1;
}
