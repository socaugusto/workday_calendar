project(WorkdayCalendarLib CXX)

# Workday Calendar as simple __Static Library__
add_library(workdaycalendarlib
    src/gregoriancalendar.cpp
    src/workdaycalendar.cpp
)

target_include_directories(workdaycalendarlib
    PUBLIC
        ./include
)

if(MSVC)
    target_compile_options(workdaycalendarlib
        PRIVATE
            /Wall
            /WX
            /std:c++20
            /permissive-
            /Zc:preprocessor
            /wd4996
            /wd4820
            /wd5045
    )
endif()