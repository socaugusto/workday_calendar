#pragma once
#include "commoncalendar.h"
#include "gregoriancalendar.h"
#include <vector>

class WorkdayCalendar
{
  public:
    WorkdayCalendar(void) = default;

    ~WorkdayCalendar(void) = default;

    void setHoliday(GregorianCalendar date);

    void setRecurringHoliday(GregorianCalendar date);

    void setWorkdayStartAndStop(GregorianCalendar startTime, GregorianCalendar stopTime);

    DateTime getWorkdayIncrement(DateTime startDate, float incrementWorkdays);

  private:
    Time start_{};
    Time stop_{};
    std::vector<Date> nonRecurringHolidays_{};
    std::vector<Date> recurringHolidays_{};
};
