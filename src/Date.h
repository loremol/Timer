#ifndef TIMER_DATE_H
#define TIMER_DATE_H


#include <string>
class Date {
public:
    Date(const int& year, const int& month, const int& day, const int& hour = 0, const int& minute = 0, const int& second = 0);
    [[nodiscard]] int getYear() const;
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getDay() const;
    [[nodiscard]] int getHour() const;
    [[nodiscard]] int getMinute() const;
    [[nodiscard]] int getSecond() const;

    [[nodiscard]] std::string getFormatted() const;

private:
    int year, month, day, hour, minute, second;
    std::string monthStrings[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    [[nodiscard]] static std::string addZeroIfNeeded(const int& value) ;

};


#endif //TIMER_DATE_H
