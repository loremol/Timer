#include "Date.h"

std::string monthNames[12] = {"January", "February", "March", "April", "May", "June", "July", "August",
                              "September", "October", "November", "December"};
std::string shortMonthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                                   "Sep", "Oct", "Nov", "Dec"};

date::date(const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &timePoint) : point(
        timePoint) {
    auto secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(point.time_since_epoch());
    auto timer = secondsSinceEpoch.count();
    timeStruct = *localtime(&timer);
}

date::date(const int &year, const int &month, const int &day, const int &hour, const int &minute, const int &second) {
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
                "Invalid timeStruct passed to timeStruct constructor. " + monthNames[month - 1] +
                " has only 30 days.");

    if (month == 2 && year % 4 != 0 && day > 28)
        throw std::invalid_argument(
                "Invalid timeStruct passed to timeStruct constructor. February has only 28 days in " +
                std::to_string(year));

    if (month == 2 && year % 4 == 0 && day > 29)
        throw std::invalid_argument(
                "Invalid timeStruct passed to timeStruct constructor. February has only 29 days in " +
                std::to_string(year));

    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_mday = day;
    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    timeStruct.tm_sec = second;
    using namespace std::chrono;
    auto msSinceEpoch = duration_cast<milliseconds>(static_cast<seconds>(mktime(&timeStruct)));
    if (timeStruct.tm_isdst) {
        timeStruct.tm_hour--;
    }
    point = time_point<system_clock, milliseconds>() + msSinceEpoch;
}

std::string date::stringifyLeadingZero(const long &value) {
    using namespace std;
    if (value >= 10) {
        return to_string(value);
    } else {
        std::string newString = "0" + to_string(value);
        return newString;
    }
}

std::string date::format(std::string format) const {
    using namespace std;
    auto hour = format.find("%H");
    if (hour != string::npos) {
        format.replace(hour, 2, stringifyLeadingZero(getHour()));
    }

    auto americanHour = format.find("%I");
    if (americanHour != string::npos) {
        std::string american{};

        if (getHour() == 0)
            american = "12";
        else if (getHour() > 12)
            american = stringifyLeadingZero(getHour() - 12);
        else
            american = stringifyLeadingZero(getHour());

        format.replace(americanHour, 2, american);
    }

    auto meridian = format.find("%p");
    if (meridian != string::npos) {
        format.replace(meridian, 2, getHour() < 12 ? "AM" : "PM");
    }

    auto minute = format.find("%M");
    if (minute != string::npos) {
        format.replace(minute, 2, stringifyLeadingZero(getMinute()));
    }

    auto second = format.find("%S");
    if (second != string::npos) {
        format.replace(second, 2, stringifyLeadingZero(getSecond()));
    }

    auto day = format.find("%d");
    if (day != string::npos) {
        format.replace(day, 2, stringifyLeadingZero(getDay()));
    }

    auto littleDay = format.find("%e");
    if (littleDay != string::npos) {
        auto before = format.substr(0, littleDay);
        auto after = format.substr(littleDay + 2);
        format = before + to_string(getDay()) + after;
    }

    auto month = format.find("%m");
    if (month != string::npos) {
        format.replace(month, 2, stringifyLeadingZero(getMonth()));
    }

    auto littleMonth = format.find("%f");
    if (littleMonth != string::npos) {
        auto before = format.substr(0, littleMonth);
        auto after = format.substr(littleMonth + 2);
        format = before + to_string(getMonth()) + after;
    }

    auto literalMonth = format.find("%B");
    if (literalMonth != string::npos) {
        auto before = format.substr(0, literalMonth);
        auto after = format.substr(literalMonth + 2);
        format = before + monthNames[getMonth() - 1] + after;
    }

    auto littleLiteralMonth = format.find("%b");
    if (littleLiteralMonth != string::npos) {
        auto before = format.substr(0, littleLiteralMonth);
        auto after = format.substr(littleLiteralMonth + 2);
        format = before + shortMonthNames[getMonth() - 1] + after;
    }

    auto year = format.find("%Y");
    if (year != string::npos) {
        auto before = format.substr(0, year);
        auto after = format.substr(year + 2);
        format = before + to_string(getYear()) + after;
    }

    auto littleYear = format.find("%y");
    if (littleYear != string::npos) {
        auto before = format.substr(0, littleYear);
        auto after = format.substr(littleYear + 2);
        format = before + to_string(getYear()).substr(2) + after;
    }

    return format;
}

const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> & date::getPoint() const {
    return point;
}

long date::getSecondsFromEpoch() const {
    using namespace std::chrono;
    return time_point_cast<seconds>(point).time_since_epoch().count();
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
