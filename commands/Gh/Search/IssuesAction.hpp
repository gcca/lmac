#pragma once

#include "commands/Gh/DateRange.hpp"

#include <concepts>
#include <format>
#include <string>

int DefaultIssuesExecutor(const std::string &cmd);

template <class Executor = decltype(&DefaultIssuesExecutor)>
  requires requires(Executor e, const std::string &cmd) {
    { e(cmd) } -> std::convertible_to<int>;
  }
class IssuesAction {
public:
  IssuesAction(DateRange range, std::string owner,
               Executor exec = DefaultIssuesExecutor)
      : range(range), owner(std::move(owner)), executor(std::move(exec)) {}

  int Run() { return executor(BuildCommand()); }

private:
  DateRange range;
  std::string owner;
  Executor executor;

  std::string BuildCommand() const {
    return std::format("gh search issues --owner={}"
                       " --json "
                       "title,body,state,url,assignees,repository,labels,"
                       "commentsCount,updatedAt,createdAt"
                       " --created {}..{}",
                       owner, FormatDate(range.start), FormatDate(range.end));
  }

  static std::string FormatDate(std::chrono::year_month_day ymd) {
    return std::format("{:%Y-%m-%d}", ymd);
  }
};

template <class Executor>
IssuesAction(DateRange, std::string, Executor) -> IssuesAction<Executor>;
