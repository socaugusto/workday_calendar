#include "simpledateformat.h"
#include "workdaycalendar.h"
#include <iostream>

int main(int, char **)
{
    WorkdayCalendar workdayCalendar{};
    GregorianCalendar startWorkday{2004, std::chrono::January, 1, 8, 0};
    GregorianCalendar stopWorkday{2004, std::chrono::January, 1, 16, 0};
    workdayCalendar.setWorkdayStartAndStop(startWorkday, stopWorkday);

    workdayCalendar.setRecurringHoliday(GregorianCalendar{2004, std::chrono::May, 17, 0, 0});
    workdayCalendar.setHoliday(GregorianCalendar{2004, std::chrono::May, 27, 0, 0});

    SimpleDateFormat f{"%d-%m-%Y %H:%M"};

    DateTime start = GregorianCalendar(2004, std::chrono::May, 24, 18, 5).getDateTime();
    float increment = -5.5f;
    DateTime result = workdayCalendar.getWorkdayIncrement(start, increment);

    std::cout << f.format(start) << " with the addition of " << increment << " working days is " << f.format(result);
}