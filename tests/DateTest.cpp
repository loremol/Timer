#include <ctime>
#include <gtest/gtest.h>
#include "../src/Date.h"

TEST(dateTest, Constructor) {
    date date1 = date(2023, 6, 1);
    date date2 = date(2000, 2, 29, 20, 17, 36);
    std::string format{"%H:%M:%S, %d/%m/%Y"};
    ASSERT_EQ(date1.getYear(), 2023);
    ASSERT_EQ(date1.getMonth(), 6);
    ASSERT_EQ(date1.getDay(), 1);
    ASSERT_EQ(date1.getHour(), 0);
    ASSERT_EQ(date1.getMinute(), 0);
    ASSERT_EQ(date1.getSecond(), 0);
    ASSERT_EQ(date1.format(format), "00:00:00, 01/06/2023");

    ASSERT_EQ(date2.getYear(), 2000);
    ASSERT_EQ(date2.getMonth(), 2);
    ASSERT_EQ(date2.getDay(), 29);
    ASSERT_EQ(date2.getHour(), 20);
    ASSERT_EQ(date2.getMinute(), 17);
    ASSERT_EQ(date2.getSecond(), 36);
    ASSERT_EQ(date2.format(format), "20:17:36, 29/02/2000");
}

TEST(dateTest, InvalidParameters) {
    // Negative parameters
    ASSERT_THROW(date(-1970, 1, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, -1, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, -29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, -20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, 20, -17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, 20, 17, -36);, std::invalid_argument);

    // Out of range parameters
    ASSERT_THROW(date(1970, 1, 35, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 13, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, 24, 17, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, 20, 60, 36);, std::invalid_argument);
    ASSERT_THROW(date(1970, 1, 29, 20, 17, 60);, std::invalid_argument);

    // Non leap years
    ASSERT_THROW(date(1999, 2, 29, 20, 17, 36);, std::invalid_argument);
}

TEST(dateTest, TimePointConstructor) {
    time_t localTime = time(nullptr);
    tm *currentTime = localtime(&localTime);
    date date{std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())};
    ASSERT_EQ(date.getYear(), currentTime->tm_year + 1900);
    ASSERT_EQ(date.getMonth(), currentTime->tm_mon + 1);
    ASSERT_EQ(date.getDay(), currentTime->tm_mday);
    ASSERT_EQ(date.getHour(), currentTime->tm_hour);
    ASSERT_EQ(date.getMinute(), currentTime->tm_min);
    ASSERT_EQ(date.getSecond(), currentTime->tm_sec);
}

TEST(dateTest, stringifyNumber) {
    srand(time(nullptr));
    for (int i = 0; i < 10; i++) {
        int random = rand() % 10;
        ASSERT_EQ("0" + std::to_string(random), date::stringifyLeadingZero(random));
    }
}
