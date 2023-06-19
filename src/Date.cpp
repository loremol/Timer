#include "Date.h"

#include <stdexcept>

date::date() {
    timeStruct.tm_year = 0;
    timeStruct.tm_mon = 0;
    timeStruct.tm_mday = 0;
    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;
}

date::date(const tm &time) : timeStruct(time) {
}

date::date(const int &year, const int &month, const int &day, const int &hour, const int &minute, const int &second)
        : timeStruct() {
    if (year < 1970)
        throw std::invalid_argument("Invalid year passed to timeStruct constructor.");

    if (month < 1 || month > 12)
        throw std::invalid_argument("Invalid month passed to timeStruct constructor.");

    if (day < 1 || day > 31)
        throw std::invalid_argument("Invalid day passed to timeStruct constructor.");

    if (hour < 0 || hour > 23)
        throw std::invalid_argument("Invalid hour passed to timeStruct constructor.");

    if (minute < 0 || minute > 59)
        throw std::invalid_argument("Invalid minute passed to timeStruct constructor.");

    if (second < 0 || second > 59)
        throw std::invalid_argument("Invalid second passed to timeStruct constructor.");

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        throw std::invalid_argument(
                "Invalid timeStruct passed to timeStruct constructor. " + monthStrings[month - 1] + " has only 30 days.");

    if (month == 2 && year % 4 != 0 && day > 28)
        throw std::invalid_argument(
                "Invalid timeStruct passed to timeStruct constructor. February has only 28 days in " + std::to_string(year));

    if (month == 2 && year % 4 == 0 && day > 29)
        throw std::invalid_argument(
                "Invalid timeStruct passed to timeStruct constructor. February has only 29 days in " + std::to_string(year));

    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_mday = day;
    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    timeStruct.tm_sec = second;
}

int date::getYear() const {
    return 1900 + timeStruct.tm_year;
}

int date::getMonth() const {
    return 1 + timeStruct.tm_mon;
}

int date::getDay() const {
    return timeStruct.tm_mday;
}

int date::getHour() const {
    return timeStruct.tm_hour;
}

int date::getMinute() const {
    return timeStruct.tm_min;
}

int date::getSecond() const {
    return timeStruct.tm_sec;
}

std::string date::getFormatted() const {
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

std::string date::addZeroIfNeeded(const int &value) {
    using namespace std;
    if (value >= 10) {
        return to_string(value);
    } else {
        return "0" + to_string(value);
    }
}
