project(WorkdayCalendarTests CXX)

# Unit Testing
add_executable(workdaycalendartests
    gregoriancalendar.cpp
)
target_link_libraries(workdaycalendartests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(workdaycalendartests)