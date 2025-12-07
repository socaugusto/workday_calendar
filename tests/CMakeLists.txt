project(WorkdayCalendarTests CXX)

# Unit Testing
add_executable(workdaycalendartests
    gregoriancalendar.cpp
    workdaycalendar.cpp
)
target_link_libraries(workdaycalendartests
    PRIVATE
        GTest::gtest_main
        workdaycalendarlib
)

include(GoogleTest)
gtest_discover_tests(workdaycalendartests)