project(WorkdayCalendarLib CXX)

# Workday Calendar as simple __Static Library__
add_library(workdaycalendarlib
    src/gregoriancalendar.cpp)

target_include_directories(workdaycalendarlib
    PUBLIC
        ./include    
)