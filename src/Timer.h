#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include <string>
#include <atomic>
#include <functional>
#include "Date.h"
#include "OptionsFrame.h"
#include "ControllerInterface.h"

enum state {
    Stopped,
    Running
};

enum timerEventId {
    TimerStarted = 20000,
    TimerTicked,
    TimerStopped
};

class timer {
public:
    timer(std::string name, int duration, controllerInterface *controller);

    timer(std::string name, const bool &state,
          const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &startPoint,
          const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> &endPoint,
          controllerInterface *controller);

    ~timer() {
        requestStop();
        while (state == Running) {}
    }

    void start();

    void requestStop();

    void sendTimerTickEvent() const;

    void setName(const std::string &s);

    void setDuration(int newDuration);

    [[nodiscard]] const bool &isRunning() const;

    [[nodiscard]] long getDuration() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] std::string formatRemainingTime(std::string format) const;

    [[nodiscard]] const date &getStartDate() const;

    [[nodiscard]] const date &getEndDate() const;

private:
    void run();

    void stop();

    void calcStartEndDates();

    void checkConstructorParameters();

    void sendTimerStoppedEvent() const;

    void sendTimerStartedEvent() const;

    controllerInterface *controller;
    std::string name;
    std::chrono::milliseconds timerDuration{0}, remainingTime{0};
    date startDate, endDate;
    bool state;
    bool stopRequested = false;
};


#endif //TIMER_TIMER_H
