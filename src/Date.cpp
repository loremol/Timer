#include "Date.h"
#include <stdexcept>

Date::Date(const int &year, const int &month, const int &day, const int &hour, const int &minute, const int &second) {
    if(year < 1970)
        throw std::invalid_argument("Invalid year passed to Date constructor.");

    if(month < 1 || month > 12)
        throw std::invalid_argument("Invalid month passed to Date constructor.");

    if(day < 1 || day > 31)
        throw std::invalid_argument("Invalid day passed to Date constructor.");

    if(hour < 0 || hour > 23)
        throw std::invalid_argument("Invalid hour passed to Date constructor.");

    if(minute < 0 || minute > 59)
        throw std::invalid_argument("Invalid minute passed to Date constructor.");

    if(second < 0 || second > 59)
        throw std::invalid_argument("Invalid second passed to Date constructor.");

    if((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        throw std::invalid_argument("Invalid date passed to Date constructor. " + monthStrings[month-1] + " has only 30 days.");

    if(month == 2 && year%4 != 0 && day > 28)
        throw std::invalid_argument("Invalid date passed to Date constructor. February has only 28 days in " + std::to_string(year));

    if(month == 2 && year%4 == 0 && day > 29)
        throw std::invalid_argument("Invalid date passed to Date constructor. February has only 29 days in " + std::to_string(year));

    this->year=year;
    this->month=month;
    this->day=day;
    this->hour=hour;
    this->minute=minute;
    this->second=second;
}

int Date::getYear() const {
    return year;
}

int Date::getMonth() const {
    return month;
}

int Date::getDay() const {
    return day;
}

int Date::getHour() const {
    return hour;
}

int Date::getMinute() const {
    return minute;
}

int Date::getSecond() const {
    return second;
}

std::string Date::getFormatted() const {
    using namespace std;
    string formattedDate;
    string monthString, dayString, hourString, minuteString, secondString;
    monthString = addZeroIfNeeded(month);
    dayString = addZeroIfNeeded(day);
    hourString = addZeroIfNeeded(hour);
    minuteString = addZeroIfNeeded(minute);
    secondString = addZeroIfNeeded(second);
    // HH:MM DD/MM/YYYY
    formattedDate = hourString + ":" + minuteString + ":" + secondString;
    formattedDate += ", ";
    formattedDate += dayString + "/" + monthString + "/" + to_string(year);
    return formattedDate;
}

std::string Date::addZeroIfNeeded(const int &value) {
    using namespace std;
    if(value >= 10) {
        return to_string(value);
    } else {
        return "0" + to_string(value);
    }
}
