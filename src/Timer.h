#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include "Date.h"
#include "Observer.h"
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

    explicit timer(std::string name, const bool &state, const time_t &startTimestamp, const time_t &endTimeStamp,
                   observer *controller);

    void start();

    void requestStop();

    void updateWhileRunning();

    void updateWhenFinished();

    void saveToFile(std::ofstream &file);

    void setName(const std::string &s);

    void setDuration(int newDuration);

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] int getDuration() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] std::string getRemainingString(const std::string &format) const;

    [[nodiscard]] date &getStartDate();

    [[nodiscard]] date &getEndDate();
private:
    void stop();

    void calcStartEndDates();

    observer *controller;
    std::string name;
    time_t duration, remaining;
    date startDate{}, endDate{};
    std::atomic<bool> state;
    std::atomic<bool> stopRequested = false;
};


#endif //TIMER_TIMER_H
