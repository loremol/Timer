#include "Timer.h"

#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

Timer::Timer(std::string name, int duration) : name(std::move(name)), duration(duration), remaining(duration),
                                               state(STOPPED) {

}

void Timer::calcStartEndDates() {
    time_t localTime = time(nullptr);
    tm *currentTime = localtime(&localTime);
    startDate = Date(*currentTime);

    time_t endTime = time(nullptr) + duration;
    tm *timerEndTime = localtime(&endTime);
    endDate = Date(*timerEndTime);
}

void Timer::start(const std::function<void()> &updateView) {
    state = RUNNING;
    calcStartEndDates();
    for (remaining = duration; remaining > 0; remaining--) {
        if (state == STOPPED)
            return;
        updateView();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    stop(updateView);
}

void Timer::stop(const std::function<void()> &updateView) {
    state = STOPPED;
    remaining = duration;
    updateView();
}

const std::string &Timer::getName() const {
    return name;
}

int Timer::getDuration() const {
    return duration;
}

int Timer::getRemaining() const {
    return remaining;
}

bool Timer::isRunning() const {
    return state;
}

bool Timer::getState() const {
    return state;
}