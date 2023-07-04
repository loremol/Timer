#ifndef TIMER_DATE_H
#define TIMER_DATE_H

#include <stdexcept>
#include <string>
#include <memory>
#include <chrono>
#include <numeric>
#include <ranges>

class date {
public:
    explicit date(const int &year = 1970, const int &month = 1, const int &day = 1, const int &hour = 0,
                  const int &minute = 0, const int &second = 0);

    explicit date(const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &timePoint);

    static std::string stringifyLeadingZero(const long &value);

    [[nodiscard]] int getYear() const;

    [[nodiscard]] int getMonth() const;

    [[nodiscard]] int getDay() const;

    [[nodiscard]] int getHour() const;

    [[nodiscard]] int getMinute() const;

    [[nodiscard]] int getSecond() const;

    [[nodiscard]] long getSecondsFromEpoch() const;

    [[nodiscard]] const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> & getPoint() const;

    [[nodiscard]] std::string format(std::string format) const;

private:
    tm timeStruct{};
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point;
};


#endif //TIMER_DATE_H
