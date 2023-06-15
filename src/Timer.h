#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H
#include "Date.h"

#include <string>

class Timer {
public:
    Timer(std::string name, int duration);

    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] int getDuration() const;
    [[nodiscard]] int getRemaining() const;
    [[nodiscard]] Date getStartDate() const;
    [[nodiscard]] Date getEndDate() const;
    [[nodiscard]] bool isRunning() const;

    void start();
    void stop();
private:
    std::string name;
    int duration, remaining; // in seconds
    Date startDate, endDate;
    bool state;
    enum State {
        STOPPED,
        RUNNING
    };
};


#endif //TIMER_TIMER_H
