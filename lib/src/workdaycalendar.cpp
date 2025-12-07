#include "workdaycalendar.h"
#include <span>

using namespace std::chrono;

namespace
{
struct WorkdayDurationsInMinutes
{
    minutes workDay;
    minutes increment;
    minutes startWorkday;
    minutes stopWorkday;
    minutes inputTime;
};

struct Holidays
{
    std::span<const Date> nonRecurring;
    std::span<const Date> recurring;
};

WorkdayDurationsInMinutes calculateTimeDuration(Time startTime, float incrementWorkdays, Time startWorkday,
                                                Time stopWorkday);

Time calculateEndTime(minutes startTime, time_point<system_clock, minutes> &timePoint,
                      const WorkdayDurationsInMinutes &time);

Date calculateEndDate(float incrementWorkdays, time_point<system_clock, minutes> timePoint, Holidays holidays);

time_point<system_clock, minutes> clampStartDate(float incrementWorkdays, time_point<system_clock, minutes> timePoint,
                                                 Holidays holidays);

time_point<system_clock, minutes> makeTimepoint(DateTime dt);
minutes clampStartTime(const WorkdayDurationsInMinutes &time);
days calculateIncrement(float incrementWorkdays);
bool isWeekend(Date date);
bool isHoliday(Date date, Holidays holidays);
} // namespace

void WorkdayCalendar::setHoliday(GregorianCalendar date)
{
    nonRecurringHolidays_.push_back(date.getDate());
}

void WorkdayCalendar::setRecurringHoliday(GregorianCalendar date)
{
    recurringHolidays_.push_back(date.getDate());
}

void WorkdayCalendar::setWorkdayStartAndStop(GregorianCalendar startTime, GregorianCalendar stopTime)
{
    start_ = startTime.getTime();
    stop_ = stopTime.getTime();
}

DateTime WorkdayCalendar::getWorkdayIncrement(DateTime startDate, float incrementWorkdays)
{
    DateTime result{};
    Holidays holidays{.nonRecurring = nonRecurringHolidays_, .recurring = recurringHolidays_};

    days increment = calculateIncrement(incrementWorkdays);
    WorkdayDurationsInMinutes timeInMinutes = calculateTimeDuration(startDate.time, incrementWorkdays, start_, stop_);

    auto correctedStartTime = clampStartTime(timeInMinutes);
    auto timePoint = makeTimepoint(startDate);
    result.time = calculateEndTime(correctedStartTime, timePoint, timeInMinutes);

    timePoint = clampStartDate(incrementWorkdays, timePoint, holidays);
    result.date = calculateEndDate(incrementWorkdays, timePoint, holidays);

    return result;
}

namespace
{
WorkdayDurationsInMinutes calculateTimeDuration(Time startTime, float incrementWorkdays, Time startWorkday,
                                                Time stopWorkday)
{
    WorkdayDurationsInMinutes result{};

    days incrementInDays{static_cast<int>(incrementWorkdays)};
    result.workDay = duration_cast<minutes>(stopWorkday.to_duration() - startWorkday.to_duration());
    result.increment =
        floor<minutes>(result.workDay * (incrementWorkdays - static_cast<float>(incrementInDays.count())));

    result.startWorkday = duration_cast<minutes>(startWorkday.to_duration());
    result.stopWorkday = duration_cast<minutes>(stopWorkday.to_duration());
    result.inputTime = duration_cast<minutes>(startTime.to_duration());

    return result;
}

minutes clampStartTime(const WorkdayDurationsInMinutes &time)
{
    auto result = time.inputTime;
    bool isStartAtEarlier = (time.inputTime < time.startWorkday);
    bool isStartAtLater = (time.inputTime > time.stopWorkday);

    if (isStartAtEarlier)
    {
        result = time.startWorkday;
    }
    else if (isStartAtLater)
    {
        result = time.stopWorkday;
    }

    return result;
}

Time calculateEndTime(minutes startTime, time_point<system_clock, minutes> &timePoint,
                      const WorkdayDurationsInMinutes &time)
{
    auto result = startTime;
    result += time.increment;

    // handle limits
    if (result < time.startWorkday)
    {
        minutes underflow = time.startWorkday - result;
        result = time.stopWorkday - underflow;
        timePoint -= days{1};
    }
    else if (result > time.stopWorkday)
    {
        minutes overflow = result - time.stopWorkday;
        result = time.startWorkday + overflow;
        timePoint += days{1};
    }

    return hh_mm_ss{result};
}

Date calculateEndDate(float incrementWorkdays, time_point<system_clock, minutes> timePoint, Holidays holidays)
{
    days increment = calculateIncrement(incrementWorkdays);

    Date current{floor<days>(timePoint)};
    days incrementInDays{static_cast<int>(incrementWorkdays)};
    while (incrementInDays.count())
    {
        timePoint += increment;
        current = Date{floor<days>(timePoint)};

        if (isWeekend(current) || isHoliday(current, holidays))
        {
            continue;
        }

        incrementInDays -= increment;
    }

    return current;
}

time_point<system_clock, minutes> clampStartDate(float incrementWorkdays, time_point<system_clock, minutes> timePoint,
                                                 Holidays holidays)
{
    days increment = calculateIncrement(incrementWorkdays);
    Date current{floor<days>(timePoint)};
    while (isWeekend(current) || isHoliday(current, holidays))
    {
        timePoint += increment;
        current = Date{floor<days>(timePoint)};
    }

    return timePoint;
}

time_point<system_clock, minutes> makeTimepoint(DateTime dt)
{
    return (sys_days(dt.date) + dt.time.hours() + dt.time.minutes());
}

days calculateIncrement(float incrementWorkdays)
{
    int sign = incrementWorkdays >= 0.0f ? 1 : -1;
    return days{sign * 1};
}

bool isWeekend(Date date)
{
    weekday wd{date};

    return (wd == Sunday || wd == Saturday);
}

bool isHoliday(Date date, Holidays holidays)
{
    for (Date holiday : holidays.nonRecurring)
    {
        if (date == holiday)
        {
            return true;
        }
    }

    for (Date holiday : holidays.recurring)
    {
        if ((holiday.month() == date.month()) && (holiday.day() == date.day()))
        {
            return true;
        }
    }
    return false;
}
} // namespace
