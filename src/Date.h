#ifndef TIMER_DATE_H
#define TIMER_DATE_H

#include <string>
#include <memory>
#include <chrono>

class date {
public:
    date() = default;

    explicit date(const tm &time);

    date(const int &year, const int &month, const int &day, const int &hour = 0, const int &minute = 0,
         const int &second = 0);

    [[nodiscard]] int getYear() const;

    [[nodiscard]] int getMonth() const;

    [[nodiscard]] int getDay() const;

    [[nodiscard]] int getHour() const;

    [[nodiscard]] int getMinute() const;

    [[nodiscard]] int getSecond() const;

    [[nodiscard]] time_t getUnixTimestamp();

    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int64_t, std::ratio<1, 1>>> getTimestamp();

    [[nodiscard]] std::string getFormatted() const;

    static std::string addZeroIfNeeded(const int &value);

private:
    tm timeStruct{};
    std::string monthStrings[12] = {"January", "February", "March", "April", "May", "June", "July", "August",
                                    "September", "October", "November", "December"};
};


#endif //TIMER_DATE_H
