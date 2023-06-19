#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include "Date.h"

#include <string>
#include <atomic>
#include <functional>
#include <mutex>

enum state {
    Stopped,
    Running
};

class timer {
public:
    explicit timer(std::string name = "", int duration = 0);

    [[nodiscard]] const std::string &getName() const;

    void setName(const std::string &s);

    void setDuration(int newDuration);

    [[nodiscard]] int getDuration() const;

    [[nodiscard]] int getRemaining() const;

    [[nodiscard]] std::string getRemainingString(const std::string &format) const;

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] bool getState() const;

    [[nodiscard]] date &getStartDate();

    [[nodiscard]] date &getEndDate();

    void start(const std::function<void()> &updateView);

    void stop(const std::function<void()> &updateView);

private:
    void calcStartEndDates();

    std::string name;
    int duration, remaining; // in seconds
    date startDate, endDate;
    bool state;
};


#endif //TIMER_TIMER_H
