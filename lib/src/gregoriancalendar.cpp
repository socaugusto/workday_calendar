#include "gregoriancalendar.h"

using namespace std::chrono;

namespace
{
uint8_t getNumberOfDaysInMonth(year, Month);
year_month_day clamp(year_month_day, Month);
hh_mm_ss<minutes> clamp(hh_mm_ss<minutes>);
} // namespace

GregorianCalendar::GregorianCalendar(DateTime dt) : date_(dt.date), time_(dt.time)
{
    date_ = clamp(date_, dt.date.month());
    time_ = clamp(time_);
}

GregorianCalendar::GregorianCalendar(int16_t y, Month m, uint8_t d, uint8_t hh, uint8_t mm)
    : GregorianCalendar(DateTime{{year_month_day{year{y}, m, day{d}}}, hh_mm_ss<minutes>{hours{hh} + minutes{mm}}})
{
}

GregorianCalendar::GregorianCalendar(year y, month m, day d, hours hh, minutes mm)
    : GregorianCalendar(DateTime{{year_month_day{y, m, d}}, hh_mm_ss<minutes>{hh + mm}})
{
}

DateTime GregorianCalendar::getDateTime(void)
{
    return {date_, time_};
}

Time GregorianCalendar::getTime(void)
{
    return time_;
}

Date GregorianCalendar::getDate(void)
{
    return date_;
}

namespace
{
uint8_t getNumberOfDaysInMonth(year y, Month m)
{
    /*
     * Source: https://github.com/cassioneri/calendar
     */

    if (m == February)
        return y.is_leap() ? 29 : 28;

    unsigned int i = static_cast<unsigned int>(m);
    return static_cast<uint8_t>(30 | (i ^ (i >> 3)));
}

year_month_day clamp(year_month_day date, Month month)
{
    uint8_t maxDayValue = getNumberOfDaysInMonth(date.year(), date.month());
    if (static_cast<unsigned int>(date.day()) > maxDayValue)
    {
        date = year_month_day{date.year(), month, day{maxDayValue}};
    }
    else if (!date.day().ok())
    {
        date = year_month_day{date.year(), month, day{1}};
    }

    if (!date.month().ok())
    {
        date = year_month_day{date.year(), January, date.day()};
    }

    return date;
}

hh_mm_ss<minutes> clamp(hh_mm_ss<minutes> time)
{
    uint8_t constexpr maxHour = 23;
    uint8_t constexpr maxMinute = 59;
    if (time.hours().count() > maxHour)
    {
        time = {hh_mm_ss<minutes>{hours{maxHour} + minutes{maxMinute}}};
    }

    return time;
}
} // namespace