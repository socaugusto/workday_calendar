#pragma once
#include <chrono>

using Month = std::chrono::month;
using Date = std::chrono::year_month_day;
using Time = std::chrono::hh_mm_ss<std::chrono::minutes>;

struct DateTime
{
    Date date;
    Time time;
};
