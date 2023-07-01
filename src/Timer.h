#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include "Date.h"
#include "OptionsFrame.h"

#include <string>
#include <atomic>
#include <functional>

enum state {
    Stopped,
    Running
};

class timer {
public:
    explicit timer(std::string name, int duration, observer *controller);

    explicit timer(std::string name, const bool &state,
                   const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &startPoint,
                   const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &endPoint,
                   observer *controller);

    void start();

    void requestStop();

    void updateWhileRunning();

    void updateWhenFinished();

    void setName(const std::string &s);

    void setDuration(int newDuration);

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] long getDuration() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] std::string formatRemainingTime(std::string format);

    [[nodiscard]] date &getStartDate();

    [[nodiscard]] date &getEndDate();

    [[nodiscard]] bool hasFinished() const;

private:
    void stop();

    void calcStartEndDates();

    void checkConstructorParameters();

    observer *controller;
    std::string name;

    std::chrono::milliseconds timerDuration{0}, remainingTime{0};
    date startDate, endDate;
    bool state, finishedStopping{false};
    std::atomic<bool> stopRequested = false;
};


#endif //TIMER_TIMER_H
