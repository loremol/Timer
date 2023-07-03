#ifndef TIMER_DATE_H
#define TIMER_DATE_H

#include <string>
#include <memory>
#include <chrono>

class date {
public:
    explicit date(const int &year = 1970, const int &month = 1, const int &day = 1, const int &hour = 0,
                  const int &minute = 0, const int &second = 0);

    explicit date(const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &timePoint);

    static std::string addZeroIfNeeded(const long &value);

    [[nodiscard]] int getYear() const;

    [[nodiscard]] int getMonth() const;

    [[nodiscard]] int getDay() const;

    [[nodiscard]] int getHour() const;

    [[nodiscard]] int getMinute() const;

    [[nodiscard]] int getSecond() const;

    [[nodiscard]] long getSecondsFromEpoch();

    [[nodiscard]] std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> getPoint();

    [[nodiscard]] std::string format(std::string format) const;

private:
    tm timeStruct{};
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point;
};


#endif //TIMER_DATE_H
