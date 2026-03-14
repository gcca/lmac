#pragma once

#include "commands/Gh/DateRange.hpp"

#include <concepts>
#include <format>
#include <string>

int DefaultPrExecutor(const std::string &cmd);

template <class Executor = decltype(&DefaultPrExecutor)>
  requires requires(Executor e, const std::string &cmd) {
    { e(cmd) } -> std::convertible_to<int>;
  }
class PrAction {
public:
  PrAction(DateRange range, Executor exec = DefaultPrExecutor)
      : range(range), executor(std::move(exec)) {}

  int Run() { return executor(BuildCommand()); }

private:
  DateRange range;
  Executor executor;

  std::string BuildCommand() const {
    return std::format("gh search prs author:@me created:{}..{}"
                       " --json number,title,createdAt,repository,body",
                       FormatDate(range.start), FormatDate(range.end));
  }

  static std::string FormatDate(std::chrono::year_month_day ymd) {
    return std::format("{:%Y-%m-%d}", ymd);
  }
};

template <class Executor> PrAction(DateRange, Executor) -> PrAction<Executor>;
