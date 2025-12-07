#include "workdaycalendar.h"
#include "gtest/gtest.h"

TEST(WorkdayCalendar, positiveIncrement_successOneDay)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2021}, January, day{4}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{5});
}

TEST(WorkdayCalendar, negativeIncrement_successOneDay)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2021}, January, day{5}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, -1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{4});
}

TEST(WorkdayCalendar, positiveIncrementThroughWeekend_success)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2025}, December, day{5}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{8});
}

TEST(WorkdayCalendar, negativeIncrementThroughWeekend_success)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2025}, December, day{8}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, -1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{5});
}

TEST(WorkdayCalendar, largePositiveIncrement_newDateNextMonth)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2021}, January, day{4}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, 20.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{1});
    ASSERT_EQ(result.date.month(), February);
}

TEST(WorkdayCalendar, negativeIncrement_newDateOnPreviousYear)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2021}, January, day{4}}, {}};

    // Act
    DateTime result = wc.getWorkdayIncrement(dt, -2.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{31});
    ASSERT_EQ(result.date.month(), December);
    ASSERT_EQ(result.date.year(), year{2020});
}

TEST(WorkdayCalendar, goesThroughRecurrentHoliday_skipsHoliday)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2023}, May, day{16}}, {}};

    // Act
    DateTime holidayMay17{Date{{}, May, day{17}}, {}}; // Year is irrelevant
    wc.setRecurringHoliday(GregorianCalendar(holidayMay17));
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{18});
}

TEST(WorkdayCalendar, goesThroughRecurrentHolidayWithYear_skipsHoliday)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2023}, May, day{16}}, {}};

    // Act
    DateTime holidayMay17{Date{year{2025}, May, day{17}}, {}}; // Year is irrelevant
    wc.setRecurringHoliday(GregorianCalendar(holidayMay17));
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{18});
}

TEST(WorkdayCalendar, goesThroughNonRecurrentHoliday_skipsHoliday)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2004}, May, day{26}}, {}};

    // Act
    DateTime holidayMay27{Date{year{2004}, May, day{27}}, {}}; // Year is relevant
    wc.setHoliday(GregorianCalendar(holidayMay27));
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{28});
}

TEST(WorkdayCalendar, holidaySetToOtherYearThanCurrent_doesNotSkipsWorkday)
{
    using namespace std::chrono;
    // Arrange
    WorkdayCalendar wc{};
    DateTime dt = {Date{year{2025}, May, day{26}}, {}};

    // Act
    DateTime holidayMay27{Date{year{2004}, May, day{27}}, {}}; // NB! Diffferent year
    wc.setHoliday(GregorianCalendar(holidayMay27));
    DateTime result = wc.getWorkdayIncrement(dt, 1.0f);

    // Assert
    ASSERT_EQ(result.date.day(), day{27});
}

class WorkdayCalendarTestFixture : public testing::Test
{
  protected:
    void SetUp() override
    {
        using namespace std::chrono;

        DateTime start = {{}, Time{hours{startHour_} + minutes{0}}};
        DateTime stop = {{}, Time{hours{startHour_ + workingTime_} + minutes{0}}};

        wc_.setWorkdayStartAndStop(start, stop);
    }

    void TearDown() override
    {
    }

    uint8_t startHour_ = 8;
    uint8_t workingTime_ = 8;
    uint8_t stopHour_ = startHour_ + workingTime_;
    WorkdayCalendar wc_{};
};

TEST_F(WorkdayCalendarTestFixture, partialDayPositiveIncrement_addsToHoursAndMinutes)
{
    using namespace std::chrono;
    // Arrange
    float increment = 0.5f;
    uint8_t incrementInHours = static_cast<uint8_t>(increment * workingTime_);
    DateTime dt = {{}, Time{hours{startHour_} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{startHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
}

TEST_F(WorkdayCalendarTestFixture, partialDayNegativeIncrement_subtractsToHoursAndMinutes)
{
    using namespace std::chrono;
    // Arrange
    float increment = -0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);
    DateTime dt = {{}, Time{hours{stopHour_} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{stopHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
}

TEST_F(WorkdayCalendarTestFixture, startEarlierThanWorkday_startsAtWorkdayHours)
{
    using namespace std::chrono;
    // Arrange
    uint8_t earlierTime = startHour_ - 2;
    float increment = 0.5f;
    uint8_t incrementInHours = static_cast<uint8_t>(increment * workingTime_);
    DateTime dt = {{}, Time{hours{earlierTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{startHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
}

TEST_F(WorkdayCalendarTestFixture, startLaterThanWorkday_startsAtWorkdayHours)
{
    using namespace std::chrono;
    // Arrange
    uint8_t laterTime = stopHour_ + 2;
    float increment = -0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);
    DateTime dt = {{}, Time{hours{laterTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{stopHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
}

TEST_F(WorkdayCalendarTestFixture, startEarlierThanWorkday_endsInPreviousDay)
{
    using namespace std::chrono;
    // Arrange
    uint8_t dayOfMonth = 9;
    uint8_t earlierTime = startHour_ - 2;
    float increment = -0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);

    // Tuesday
    Date tuesday{year{2025} / December / day{dayOfMonth}};
    DateTime dt = {tuesday, Time{hours{earlierTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{stopHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
    ASSERT_EQ(result.date.day(), day{dayOfMonth - 1u});
    ASSERT_EQ(result.date.month(), tuesday.month());
}

TEST_F(WorkdayCalendarTestFixture, startLaterThanWorkday_endsInNextDay)
{
    using namespace std::chrono;
    // Arrange
    uint8_t dayOfMonth = 9;
    uint8_t laterTime = stopHour_ + 2;
    float increment = 0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);

    // Tuesday
    Date tuesday{year{2025} / December / day{dayOfMonth}};
    DateTime dt = {tuesday, Time{hours{laterTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{startHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
    ASSERT_EQ(result.date.day(), day{dayOfMonth + 1u});
    ASSERT_EQ(result.date.month(), tuesday.month());
}

TEST_F(WorkdayCalendarTestFixture, startEarlierThanWorkdayMonday_endsInPreviousFriday)
{
    using namespace std::chrono;
    // Arrange
    uint8_t dayOfMonth = 8;
    uint8_t earlierTime = startHour_ - 2;
    float increment = -0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);

    // Monday
    Date monday{year{2025} / December / day{dayOfMonth}};
    DateTime dt = {monday, Time{hours{earlierTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{stopHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
    ASSERT_EQ(result.date.day(), day{dayOfMonth - 3u});
    ASSERT_EQ(result.date.month(), monday.month());
}

TEST_F(WorkdayCalendarTestFixture, startLaterThanWorkdayFriday_endsInNextMonday)
{
    using namespace std::chrono;
    // Arrange
    uint8_t dayOfMonth = 5;
    uint8_t laterTime = stopHour_ + 2;
    float increment = 0.5f;
    int8_t incrementInHours = static_cast<int8_t>(increment * workingTime_);

    // Friday
    Date friday{year{2025} / December / day{dayOfMonth}};
    DateTime dt = {friday, Time{hours{laterTime} + minutes{0}}};

    // Act
    DateTime result = wc_.getWorkdayIncrement(dt, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{startHour_ + incrementInHours});
    ASSERT_EQ(result.time.minutes(), minutes{0});
    ASSERT_EQ(result.date.day(), day{dayOfMonth + 3u});
    ASSERT_EQ(result.date.month(), friday.month());
}

struct KataScenario
{
    float incrementWorkdays;
    uint8_t startHour, startMinute;
    int16_t resultYear;
    Month resultMonth;
    uint8_t resultDay, resultHour, resultMinute;
};

class WorkdayCalendarKata : public testing::TestWithParam<KataScenario>
{
};

TEST_P(WorkdayCalendarKata, scenarios)
{
    using namespace std::chrono;
    // Arrange
    auto [increment, sh, sm, ry, rm, rd, rh, rmin] = GetParam();

    WorkdayCalendar workdayCalendar{};
    GregorianCalendar startWorkday{2004, January, 1, 8, 0};
    GregorianCalendar stopWorkday{2004, January, 1, 16, 0};
    workdayCalendar.setWorkdayStartAndStop(startWorkday, stopWorkday);

    workdayCalendar.setRecurringHoliday(GregorianCalendar{2004, May, 17, 0, 0});
    workdayCalendar.setHoliday(GregorianCalendar{2004, May, 27, 0, 0});

    // Act
    DateTime start = GregorianCalendar(2004, May, 24, sh, sm).getDateTime();
    DateTime result = workdayCalendar.getWorkdayIncrement(start, increment);

    // Assert
    ASSERT_EQ(result.time.hours(), hours{rh});
    ASSERT_EQ(result.time.minutes(), minutes{rmin});
    ASSERT_EQ(result.date.day(), day{rd});
    ASSERT_EQ(result.date.month(), rm);
    ASSERT_EQ(result.date.year(), year{ry});
}

INSTANTIATE_TEST_SUITE_P(KataScenarios, WorkdayCalendarKata,
                         testing::Values(KataScenario{-5.5f, 18, 5, 2004, std::chrono::May, 14, 12, 0},
                                         KataScenario{44.723656f, 19, 3, 2004, std::chrono::July, 27, 13, 47},
                                         KataScenario{-6.7470217f, 18, 3, 2004, std::chrono::May, 13, 10, 1},
                                         KataScenario{12.782709f, 8, 3, 2004, std::chrono::June, 10, 14, 18},
                                         KataScenario{8.276628f, 7, 3, 2004, std::chrono::June, 4, 10, 12}));
