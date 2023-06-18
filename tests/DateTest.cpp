#include <cmath>
#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "../src/Date.h"

TEST(Date, Constructor) {
    Date date1 = Date(2023, 6, 1);
    Date date2 = Date(2000, 2, 29, 20, 17, 36);

    ASSERT_EQ(date1.getYear(), 2023);
    ASSERT_EQ(date1.getMonth(), 6);
    ASSERT_EQ(date1.getDay(), 1);
    ASSERT_EQ(date1.getHour(), 0);
    ASSERT_EQ(date1.getMinute(), 0);
    ASSERT_EQ(date1.getSecond(), 0);
    ASSERT_EQ(date1.getFormatted(), "00:00:00, 01/06/2023");

    ASSERT_EQ(date2.getYear(), 2000);
    ASSERT_EQ(date2.getMonth(), 2);
    ASSERT_EQ(date2.getDay(), 29);
    ASSERT_EQ(date2.getHour(), 20);
    ASSERT_EQ(date2.getMinute(), 17);
    ASSERT_EQ(date2.getSecond(), 36);
    ASSERT_EQ(date2.getFormatted(), "20:17:36, 29/02/2000");
}

TEST(Date, InvalidDatesToConstructor) {
    // Negative parameters
    ASSERT_THROW(Date(-1970, 1, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, -1, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, -29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, -20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, 20, -17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, 20, 17, -36);, std::invalid_argument);

    // Out of range parameters
    ASSERT_THROW(Date(1970, 1, 35, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 13, 29, 20, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, 24, 17, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, 20, 60, 36);, std::invalid_argument);
    ASSERT_THROW(Date(1970, 1, 29, 20, 17, 60);, std::invalid_argument);

    // Non leap years
    ASSERT_THROW(Date(1999, 2, 29, 20, 17, 36);, std::invalid_argument);
}

TEST(Date, TMConstructor) {
    time_t localTime = time(nullptr);
    tm *currentTime = localtime(&localTime);
    Date date = Date(*currentTime);
    ASSERT_EQ(date.getYear(), currentTime->tm_year+1900);
    ASSERT_EQ(date.getMonth(), currentTime->tm_mon+1);
    ASSERT_EQ(date.getDay(), currentTime->tm_mday);
    ASSERT_EQ(date.getHour(), currentTime->tm_hour);
    ASSERT_EQ(date.getMinute(), currentTime->tm_min);
    ASSERT_EQ(date.getSecond(), currentTime->tm_sec);
}