#pragma once

#include <chrono>

struct DateRange {
  std::chrono::year_month_day start;
  std::chrono::year_month_day end;
};
