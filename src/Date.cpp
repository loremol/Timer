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

std::string date::formatDate(const std::string &format) const {
    using namespace std;
    string formattedDate{format};
    auto hour = format.find("%H");
    if (hour != string::npos) {
        formattedDate.replace(hour, 2, addZeroIfNeeded(getHour()));
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

        formattedDate.replace(americanHour, 2, american);
    }

    auto meridian = format.find("%p");
    if (meridian != string::npos) {
        formattedDate.replace(meridian, 2, getHour() < 12 ? "AM" : "PM");
    }

    auto minute = format.find("%M");
    if (minute != string::npos) {
        formattedDate.replace(minute, 2, addZeroIfNeeded(getMinute()));
    }

    auto second = format.find("%S");
    if (second != string::npos) {
        formattedDate.replace(second, 2, addZeroIfNeeded(getSecond()));
    }

    auto day = format.find("%d");
    if (day != string::npos) {
        formattedDate.replace(day, 2, addZeroIfNeeded(getDay()));
    }

    auto littleDay = format.find("%e");
    if (littleDay != string::npos) {
        auto before = formattedDate.substr(0, littleDay);
        auto after = formattedDate.substr(littleDay + 2);
        formattedDate = before + to_string(getDay()) + after;
    }

    auto month = format.find("%m");
    if (month != string::npos) {
        formattedDate.replace(month, 2, addZeroIfNeeded(getMonth()));
    }

    auto littleMonth = format.find("%f");
    if (littleMonth != string::npos) {
        auto before = formattedDate.substr(0, littleMonth);
        auto after = formattedDate.substr(littleMonth + 2);
        formattedDate = before + to_string(getMonth()) + after;
    }

    auto literalMonth = format.find("%B");
    if (literalMonth != string::npos) {
        auto before = formattedDate.substr(0, literalMonth);
        auto after = formattedDate.substr(literalMonth + 2);
        formattedDate = before + monthNames[getMonth() - 1] + after;
    }

    auto littleLiteralMonth = format.find("%b");
    if (littleLiteralMonth != string::npos) {
        auto before = formattedDate.substr(0, littleLiteralMonth);
        auto after = formattedDate.substr(littleLiteralMonth + 2);
        formattedDate = before + shortMonthNames[getMonth() - 1] + after;
    }

    auto year = format.find("%Y");
    if (year != string::npos) {
        auto before = formattedDate.substr(0, year);
        auto after = formattedDate.substr(year + 2);
        formattedDate = before + to_string(getYear()) + after;
    }

    auto littleYear = format.find("%y");
    if (littleYear != string::npos) {
        auto before = formattedDate.substr(0, littleYear);
        auto after = formattedDate.substr(littleYear + 2);
        formattedDate = before + to_string(getYear()).substr(2) + after;
    }

    return formattedDate;
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
