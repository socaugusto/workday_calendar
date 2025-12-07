#include "gregoriancalendar.h"
#include <gtest/gtest.h>

TEST(GregorianCalendar, simpleConstruction_createSuccessfull)
{
    using namespace std::chrono;
    // Arrange
    // Act
    GregorianCalendar gc = GregorianCalendar{{}, {}, {}, {}, {}};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_TRUE(dt.date.ok());
}

TEST(GregorianCalendar, typeSafeComposedConstruction_createSuccessfull)
{
    using namespace std::chrono;
    // Arrange
    Date date{year{2025}, month{December}, day{10}};
    Time time{hours{9} + minutes{30}};
    DateTime input{date, time};

    // Act
    GregorianCalendar gc = GregorianCalendar{input};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_EQ(dt.date.day(), date.day());
    EXPECT_EQ(dt.date.month(), date.month());
    EXPECT_EQ(dt.date.year(), date.year());
    EXPECT_EQ(dt.time.hours(), time.hours());
    EXPECT_EQ(dt.time.minutes(), time.minutes());
}

TEST(GregorianCalendar, typeSafeExtendConstruction_createSuccessfull)
{
    using namespace std::chrono;
    // Arrange
    year y{2021};
    month m{March};
    day d{29};
    hours hh{3};
    minutes mm{33};

    // Act
    GregorianCalendar gc = GregorianCalendar{y, m, d, hh, mm};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_EQ(dt.date.day(), d);
    EXPECT_EQ(dt.date.month(), m);
    EXPECT_EQ(dt.date.year(), y);
    EXPECT_EQ(dt.time.hours(), hh);
    EXPECT_EQ(dt.time.minutes(), mm);
}

TEST(GregorianCalendar, todaysDate_createSuccessfull)
{
    using namespace std::chrono;
    // Arrange
    auto today = system_clock::now();
    auto ymd = year_month_day{floor<days>(today)};

    year y = ymd.year();
    month m = ymd.month();
    day d = ymd.day();

    auto hhmm = hh_mm_ss<minutes>{floor<minutes>(today) - floor<days>(today)};
    hours hh = hhmm.hours();
    minutes mm = hhmm.minutes();

    // Act
    int16_t year = static_cast<int>(y);
    uint8_t day = static_cast<unsigned int>(d);
    Month month = static_cast<Month>(static_cast<unsigned int>(m));

    uint8_t hour = static_cast<uint8_t>(hh.count());
    uint8_t minute = static_cast<uint8_t>(mm.count());

    GregorianCalendar gc = GregorianCalendar{year, month, day, hour, minute};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_EQ(dt.date.day(), d);
    EXPECT_EQ(dt.date.month(), m);
    EXPECT_EQ(dt.date.year(), y);
    EXPECT_EQ(dt.time.hours(), hh);
    EXPECT_EQ(dt.time.minutes(), mm);
}

TEST(GregorianCalendar, invalidFebruary29_clampTo28)
{
    using namespace std::chrono;
    // Arrange
    unsigned char d29 = 29;

    // Act
    GregorianCalendar gc = GregorianCalendar{2025, February, d29, {}, {}};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_TRUE(dt.date.ok());
    EXPECT_EQ(dt.date.month(), February);
    EXPECT_EQ(dt.date.day(), day{d29 - 1u});
}

TEST(GregorianCalendar, invalidHour_clampToMaxTime)
{
    using namespace std::chrono;
    // Arrange
    unsigned char h24 = 24;

    // Act
    GregorianCalendar gc = GregorianCalendar{{}, {}, {}, h24, {}};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_EQ(dt.time.hours(), hours{23});
    EXPECT_EQ(dt.time.minutes(), minutes{59});
}

TEST(GregorianCalendar, invalidTime_clampToMaxTime)
{
    using namespace std::chrono;
    // Arrange
    unsigned char m250 = 250;
    unsigned char h23 = 23;

    // Act
    GregorianCalendar gc = GregorianCalendar{{}, {}, {}, h23, m250};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_EQ(dt.time.hours(), hours{h23});
    EXPECT_EQ(dt.time.minutes(), minutes{59});
}

/**
 * Tests months with 30 days to ensure the day is clamped.
 */
struct GregorianCalendar30DaysMonths : public testing::TestWithParam<Month>
{
};

TEST_P(GregorianCalendar30DaysMonths, invalidDay31thInApril_clampTo30)
{
    using namespace std::chrono;
    // Arrange
    unsigned char d31 = 31;
    Month m30 = GetParam();

    // Act
    GregorianCalendar gc = GregorianCalendar{2025, m30, d31, {}, {}};
    DateTime dt = gc.getDateTime();

    // Assert
    EXPECT_TRUE(dt.date.ok());
    EXPECT_EQ(dt.date.month(), m30);
    EXPECT_EQ(dt.date.day(), day{d31 - 1u});
}

INSTANTIATE_TEST_SUITE_P(AprilJuneSeptemberNovember,
                         GregorianCalendar30DaysMonths,
                         testing::Values(std::chrono::April,
                                         std::chrono::June,
                                         std::chrono::September,
                                         std::chrono::November));

/*
 * Additional tests covering months with 31 days, boundary years (minimum and maximum),
 * leap years, and other edge cases should be implemented to complete test coverage.
 * Given the scope of this kata, the existing tests are sufficient for demonstration purposes.
 */
