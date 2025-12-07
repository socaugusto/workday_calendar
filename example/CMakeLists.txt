project(WorkdayCalendarExample CXX)

add_executable(workdaycalendarexample
    main.cpp
)
target_link_libraries(workdaycalendarexample
    PRIVATE
        workdaycalendarlib
)