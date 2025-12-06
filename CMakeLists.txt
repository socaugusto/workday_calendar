#[[ GoogleTest module got the gtest_discover_tests() in version 3.10,
#   adding this version as minimum to allow dynamic test discovery.
]]#
cmake_minimum_required(VERSION 3.10)
project(WorkdayCalendar LANGUAGES CXX)

# C++ language requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest
# https://google.github.io/googletest/quickstart-cmake.html
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0  # Latest stable
)
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

add_subdirectory(lib)
add_subdirectory(tests)
