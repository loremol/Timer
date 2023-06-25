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
    explicit timer(std::string name, int duration);
    explicit timer(std::string name, const bool &state, const time_t &startTimestamp, const time_t &endTimeStamp);

    ~timer() {
        stop([](){});
    }

    [[nodiscard]] const std::string &getName() const;

    void setName(const std::string &s);

    void setDuration(int newDuration);

    [[nodiscard]] int getDuration() const;

    [[nodiscard]] int getRemaining() const;

    [[nodiscard]] std::string getRemainingString(const std::string &format) const;

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] date &getStartDate();

    [[nodiscard]] date &getEndDate();

    void start(const std::function<void()> &updateView);

    void requestStop();

    void stop(const std::function<void()> &updateView);

    void saveToFile(std::ofstream &file);

private:
    void calcStartEndDates();

    std::string name;
    time_t duration, remaining;
    date startDate{}, endDate{};
    std::atomic<bool> state;
    std::atomic<bool> stopRequested = false;
};


#endif //TIMER_TIMER_H
