#pragma once
#include <chrono>
#include <cstdint>

using Month = std::chrono::month;
using Date = std::chrono::year_month_day;
using Time = std::chrono::hh_mm_ss<std::chrono::minutes>;

struct DateTime
{
    Date date;
    Time time;
};

/**
 * @brief Class which represents a time point
 *
 * Provides a simple interface to construct time points which
 * can then be used in the Workday Calendar class.
 *
 */
class GregorianCalendar
{
  public:
    GregorianCalendar(int16_t year, Month month, uint8_t day, uint8_t hour, uint8_t minute);

    GregorianCalendar(std::chrono::year, std::chrono::month, std::chrono::day, std::chrono::hours,
                      std::chrono::minutes);

    GregorianCalendar(DateTime);

    GregorianCalendar(void) = delete;

    ~GregorianCalendar(void) = default;

    DateTime getDateTime(void);

    Time getTime(void);

    Date getDate(void);

  private:
    std::chrono::year_month_day date_{};
    std::chrono::hh_mm_ss<std::chrono::minutes> time_{};
};
