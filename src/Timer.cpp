#include "Timer.h"

#include <cmath>
#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

timer::timer(std::string name, int duration) : name(std::move(name)), duration(duration), remaining(duration),
                                               state(Stopped) {
    calcStartEndDates();
}

timer::timer(std::string name, const bool &state, const time_t &startTimestamp, const time_t &endTimeStamp)
        : name(std::move(name)), state(state) {
    startDate = date(*localtime(&startTimestamp));
    endDate = date(*localtime(&endTimeStamp));
    duration = endTimeStamp - startTimestamp;
    if(isRunning()) {
        time_t currentTime = time(nullptr);
        remaining = endTimeStamp - currentTime;
    } else {
        remaining = duration;
    }
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
    if(!isRunning()) {
        calcStartEndDates();
    }
    state = Running;
    for (; remaining > 0; remaining--) {
        for (int i = 0; i < 40; i++) {
            updateView();
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            if (stopRequested) {
                stopRequested = false;
                stop(updateView);
                return;
            }
        }
    }
    stop(updateView);
}

void timer::stop(const std::function<void()> &updateView) {
    remaining = duration;
    state = Stopped;
    updateView();
}

void timer::requestStop() {
    if (isRunning())
        stopRequested = true;
}

void timer::saveToFile(std::ofstream &file) {
    file << state << "\t" << name << "\t";
    file << startDate.getUnixTimestamp() << "\t" << endDate.getUnixTimestamp() << std::endl;
}

void timer::setName(const std::string &s) {
    name = s;
}

void timer::setDuration(int newDuration) {
    duration = newDuration;
    if (state == Stopped)
        remaining = newDuration;
    calcStartEndDates();
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

    if (!isRunning())
        f(static_cast<int>(duration));
    else
        f(static_cast<int>(remaining));

    return s;
}

date &timer::getStartDate() {
    return startDate;
}

date &timer::getEndDate() {
    return endDate;
}

const std::string &timer::getName() const {
    return name;
}

int timer::getDuration() const {
    return static_cast<int>(duration);
}

int timer::getRemaining() const {
    return static_cast<int>(remaining);
}

bool timer::isRunning() const {
    return state;
}
