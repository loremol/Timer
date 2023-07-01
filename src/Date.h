#ifndef TIMER_DATE_H
#define TIMER_DATE_H

#include <string>
#include <memory>
#include <chrono>

class date {
public:
    date() = default;

    explicit date(const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &timePoint);

    [[nodiscard]] int getYear() const;

    [[nodiscard]] int getMonth() const;

    [[nodiscard]] int getDay() const;

    [[nodiscard]] int getHour() const;

    [[nodiscard]] int getMinute() const;

    [[nodiscard]] int getSecond() const;

    [[nodiscard]] long getSecondsFromEpoch();

    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> getPoint();

    [[nodiscard]] std::string formatDate(std::string format) const;

    static std::string addZeroIfNeeded(const long &value);

private:
    tm timeStruct{};
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point;
};


#endif //TIMER_DATE_H
