#include "Timer.h"

#include <cmath>
#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

timer::timer(std::string name, int duration) : name(std::move(name)), duration(duration), remaining(duration),
                                               state(Stopped) {

}

void timer::calcStartEndDates() {
    time_t localTime = time(nullptr);
    tm *currentTime = localtime(&localTime);
    startDate = date(*currentTime);

    time_t endTime = time(nullptr) + duration;
    tm *timerEndTime = localtime(&endTime);
    endDate = date(*timerEndTime);
}

void timer::start(const std::function<void()> &updateView) {
    state = Running;
    calcStartEndDates();
    for (remaining = duration; remaining > 0; remaining--) {
        if (state == Stopped)
            return;
        updateView();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    stop(updateView);
}

void timer::stop(const std::function<void()> &updateView) {
    state = Stopped;
    remaining = duration;
    updateView();
}

const std::string &timer::getName() const {
    return name;
}

int timer::getDuration() const {
    return duration;
}

int timer::getRemaining() const {
    return remaining;
}

bool timer::isRunning() const {
    return state;
}

bool timer::getState() const {
    return state;
}

std::string timer::getRemainingString(const std::string &format) const {
    std::string s;
    const auto f = [&format, &s](int totalSeconds) {
        if (format == "HH:MM:SS") {
            int hours = std::floor(static_cast<float>(totalSeconds) / 3600);
            int minutes = std::floor((totalSeconds - (hours * 3600)) / 60);
            int seconds = totalSeconds - (hours * 3600) - (minutes * 60);

            s += date::addZeroIfNeeded(hours) + ":";
            s += date::addZeroIfNeeded(minutes) + ":";
            s += date::addZeroIfNeeded(seconds);
        } else {
            s = "Format not supported.";
        }
    };

    if (getState() == Stopped)
        f(duration);
    else
        f(remaining);

    return s;
}

void timer::setName(const std::string &s) {
    name = s;
}

void timer::setDuration(int newDuration) {
    timer::duration = newDuration;
    if (state == Stopped)
        remaining = newDuration;
}

date &timer::getStartDate() {
    return startDate;
}

date &timer::getEndDate() {
    return endDate;
}
