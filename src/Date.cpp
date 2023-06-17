#include "Date.h"

#include <stdexcept>

Date::Date() : date() {
    date.tm_year = 0;
    date.tm_mon = 0;
    date.tm_mday = 0;
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
}

Date::Date(tm time) : date(time) {
}

Date::Date(const int &year, const int &month, const int &day, const int &hour, const int &minute, const int &second)
        : date() {
    if (year < 1970)
        throw std::invalid_argument("Invalid year passed to Date constructor.");

    if (month < 1 || month > 12)
        throw std::invalid_argument("Invalid month passed to Date constructor.");

    if (day < 1 || day > 31)
        throw std::invalid_argument("Invalid day passed to Date constructor.");

    if (hour < 0 || hour > 23)
        throw std::invalid_argument("Invalid hour passed to Date constructor.");

    if (minute < 0 || minute > 59)
        throw std::invalid_argument("Invalid minute passed to Date constructor.");

    if (second < 0 || second > 59)
        throw std::invalid_argument("Invalid second passed to Date constructor.");

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        throw std::invalid_argument(
                "Invalid date passed to Date constructor. " + monthStrings[month - 1] + " has only 30 days.");

    if (month == 2 && year % 4 != 0 && day > 28)
        throw std::invalid_argument(
                "Invalid date passed to Date constructor. February has only 28 days in " + std::to_string(year));

    if (month == 2 && year % 4 == 0 && day > 29)
        throw std::invalid_argument(
                "Invalid date passed to Date constructor. February has only 29 days in " + std::to_string(year));

    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_hour = hour;
    date.tm_min = minute;
    date.tm_sec = second;
}

int Date::getYear() const {
    return 1900 + date.tm_year;
}

int Date::getMonth() const {
    return 1 + date.tm_mon;
}

int Date::getDay() const {
    return date.tm_mday;
}

int Date::getHour() const {
    return date.tm_hour;
}

int Date::getMinute() const {
    return date.tm_min;
}

int Date::getSecond() const {
    return date.tm_sec;
}

std::string Date::getFormatted() const {
    using namespace std;
    string formattedDate;
    string monthString, dayString, hourString, minuteString, secondString;
    monthString = addZeroIfNeeded(getMonth());
    dayString = addZeroIfNeeded(getDay());
    hourString = addZeroIfNeeded(getHour());
    minuteString = addZeroIfNeeded(getMinute());
    secondString = addZeroIfNeeded(getSecond());
    // HH:MM DD/MM/YYYY
    formattedDate = hourString + ":" + minuteString + ":" + secondString;
    formattedDate += ", ";
    formattedDate += dayString + "/" + monthString + "/" + to_string(getYear());
    return formattedDate;
}

std::string Date::addZeroIfNeeded(const int &value) {
    using namespace std;
    if (value >= 10) {
        return to_string(value);
    } else {
        return "0" + to_string(value);
    }
}
