#include "Date.h"

#include <stdexcept>

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

std::string date::addZeroIfNeeded(const long &value) {
    using namespace std;
    if (value >= 10) {
        return to_string(value);
    } else {
        std::string newString = "0" + to_string(value);
        return newString;
    }
}

std::string date::formatDate(std::string format) const {
    using namespace std;
    auto hour = format.find("%H");
    if (hour != string::npos) {
        format.replace(hour, 2, addZeroIfNeeded(getHour()));
    }

    auto americanHour = format.find("%I");
    if (americanHour != string::npos) {
        std::string american{};

        if (getHour() == 0)
            american = "12";
        else if (getHour() > 12 && getHour() != 0)
            american = addZeroIfNeeded(getHour() - 12);
        else
            american = addZeroIfNeeded(getHour());

        format.replace(americanHour, 2, american);
    }

    auto meridian = format.find("%p");
    if (meridian != string::npos) {
        format.replace(meridian, 2, getHour() < 12 ? "AM" : "PM");
    }

    auto minute = format.find("%M");
    if (minute != string::npos) {
        format.replace(minute, 2, addZeroIfNeeded(getMinute()));
    }

    auto second = format.find("%S");
    if (second != string::npos) {
        format.replace(second, 2, addZeroIfNeeded(getSecond()));
    }

    auto day = format.find("%d");
    if (day != string::npos) {
        format.replace(day, 2, addZeroIfNeeded(getDay()));
    }

    auto littleDay = format.find("%e");
    if (littleDay != string::npos) {
        auto before = format.substr(0, littleDay);
        auto after = format.substr(littleDay + 2);
        format = before + to_string(getDay()) + after;
    }

    auto month = format.find("%m");
    if (month != string::npos) {
        format.replace(month, 2, addZeroIfNeeded(getMonth()));
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

long date::getSecondsFromEpoch() {
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

using namespace std::chrono;

time_point<system_clock, milliseconds>
date::getPoint() {
    return point;
}
