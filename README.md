# Workday Calendar

A modern C++20 library for calculating workday increments based on configurable working hours, holidays, and recurring holidays. The library automatically handles weekends and provides precise date/time calculations down to the minute.

## Features

- **Workday Increment Calculation** — Add or subtract fractional workdays from a given date/time
- **Configurable Working Hours** — Define custom start and stop times for the workday
- **Holiday Support** — Set one-time holidays for specific dates
- **Recurring Holidays** — Define holidays that repeat every year (e.g., national holidays)
- **Weekend Handling** — Automatically skips Saturdays and Sundays
- **Fractional Days** — Supports partial workday increments (e.g., 0.5 days, 2.25 days)
- **Bidirectional** — Calculate both forward and backward in time with positive/negative increments
- **Date Formatting** — Includes a simple date formatter using C++20 `std::format`

## Requirements

- **C++20** compatible compiler
- **CMake** 3.10 or later
- **Ninja** build system (recommended)

### Tested Compilers

| Compiler | Version |
|----------|---------|
| MSVC     | 19.44.35207.1 |
| GCC      | 13.3.0 |

## Project Structure

```
workday_calendar/
├── CMakeLists.txt          # Root CMake configuration
├── CMakePresets.json       # Build presets for Windows/Linux
├── LICENSE                 # MIT License
├── README.md
├── lib/                    # Library source code
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── commoncalendar.h      # Common type definitions
│   │   ├── gregoriancalendar.h   # Date/time representation
│   │   ├── simpledateformat.h    # Date formatting utility
│   │   └── workdaycalendar.h     # Main workday calculator
│   └── src/
│       ├── gregoriancalendar.cpp
│       └── workdaycalendar.cpp
├── example/                # Usage example
│   ├── CMakeLists.txt
│   └── main.cpp
└── tests/                  # Unit tests (GoogleTest)
    ├── CMakeLists.txt
    ├── gregoriancalendar.cpp
    └── workdaycalendar.cpp
```

## Building

### Using CMake Presets (Recommended)

The project includes CMake presets for easy building on Windows and Linux.

#### Linux (GCC)

```bash
# Debug build
cmake --preset linux-gcc-debug
cmake --build --preset linux-gcc-debug-build

# Release build
cmake --preset linux-gcc-release
cmake --build --preset linux-gcc-release-build
```

#### Windows (MSVC)

```bash
# Debug build
cmake --preset msvc-x64-debug
cmake --build --preset msvc-x64-debug-build

# Release build
cmake --preset msvc-x64-release
cmake --build --preset msvc-x64-release-build
```

### Manual Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### IDE Support

The project works seamlessly with:
- **Visual Studio Code** (with CMake Tools extension)
- **Visual Studio** (open folder or CMake project)
- **CLion**

## Usage

### Basic Example

```cpp
#include "simpledateformat.h"
#include "workdaycalendar.h"
#include <iostream>

int main()
{
    // Create a workday calendar
    WorkdayCalendar workdayCalendar{};

    // Set working hours: 8:00 AM to 4:00 PM
    GregorianCalendar startWorkday{2004, std::chrono::January, 1, 8, 0};
    GregorianCalendar stopWorkday{2004, std::chrono::January, 1, 16, 0};
    workdayCalendar.setWorkdayStartAndStop(startWorkday, stopWorkday);

    // Set a recurring holiday (May 17th every year)
    workdayCalendar.setRecurringHoliday(GregorianCalendar{2004, std::chrono::May, 17, 0, 0});

    // Set a one-time holiday
    workdayCalendar.setHoliday(GregorianCalendar{2004, std::chrono::May, 27, 0, 0});

    // Create a date formatter
    SimpleDateFormat f{"%d-%m-%Y %H:%M"};

    // Calculate workday increment
    DateTime start = GregorianCalendar(2004, std::chrono::May, 24, 18, 5).getDateTime();
    float increment = -5.5f;  // Go back 5.5 workdays
    DateTime result = workdayCalendar.getWorkdayIncrement(start, increment);

    std::cout << f.format(start) << " with the addition of " << increment 
              << " working days is " << f.format(result) << std::endl;
    // Output: 24-05-2004 18:05 with the addition of -5.5 working days is 14-05-2004 12:00
}
```

## API Reference

### `WorkdayCalendar`

The main class for workday calculations.

```cpp
class WorkdayCalendar {
    // Set working hours for each day
    void setWorkdayStartAndStop(GregorianCalendar startTime, GregorianCalendar stopTime);

    // Add a one-time holiday (specific year, month, day)
    void setHoliday(GregorianCalendar date);

    // Add a recurring holiday (same month/day every year)
    void setRecurringHoliday(GregorianCalendar date);

    // Calculate the resulting date/time after adding workdays
    DateTime getWorkdayIncrement(DateTime startDate, float incrementWorkdays);
};
```

### `GregorianCalendar`

Represents a point in time with multiple construction options.

```cpp
class GregorianCalendar {
    // Construct with primitive types
    GregorianCalendar(int16_t year, Month month, uint8_t day, uint8_t hour, uint8_t minute);

    // Construct with C++20 chrono types
    GregorianCalendar(std::chrono::year, std::chrono::month, std::chrono::day,
                      std::chrono::hours, std::chrono::minutes);

    // Construct from DateTime
    GregorianCalendar(DateTime);

    // Accessors
    DateTime getDateTime();
    Time getTime();
    Date getDate();
};
```

### `SimpleDateFormat`

A utility class for formatting dates using `std::format` syntax.

```cpp
class SimpleDateFormat {
    constexpr SimpleDateFormat(std::string_view fmt);
    std::string format(const DateTime& dt) const;
};
```

### Common Types

```cpp
using Month = std::chrono::month;
using Date = std::chrono::year_month_day;
using Time = std::chrono::hh_mm_ss<std::chrono::minutes>;

struct DateTime {
    Date date;
    Time time;
};
```

## Running Tests

The project uses [GoogleTest](https://github.com/google/googletest) v1.14.0 for unit testing, which is automatically fetched during the CMake configuration.

```bash
# Build tests
cmake --preset linux-gcc-debug
cmake --build --preset linux-gcc-debug-build

# Run tests
cd build/linux-gcc-debug
ctest --output-on-failure

# Or run the test executable directly
./workdaycalendartests
```

### Test Coverage

The test suite includes:
- **GregorianCalendar tests** — Construction, date validation, clamping invalid dates
- **WorkdayCalendar tests** — Positive/negative increments, weekend handling, holidays, partial days
- **Kata scenarios** — Real-world test cases with various increment values

## How It Works

1. **Time Clamping**: If the start time is outside working hours, it's clamped to the nearest boundary
2. **Fractional Days**: Partial workdays are converted to minutes based on the configured workday length
3. **Date Iteration**: The algorithm iterates day by day, skipping weekends and holidays
4. **Overflow/Underflow**: When time calculations overflow or underflow the workday boundaries, the date is adjusted accordingly

### Example Calculations

| Start Date/Time | Increment | Result |
|-----------------|-----------|--------|
| May 24, 2004 18:05 | -5.5 | May 14, 2004 12:00 |
| May 24, 2004 19:03 | +44.72 | July 27, 2004 13:47 |
| May 24, 2004 08:03 | +12.78 | June 10, 2004 14:18 |

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License
Copyright (c) 2025 socaugusto
```
