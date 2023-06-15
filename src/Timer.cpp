#include "Timer.h"

#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

Timer::Timer(std::string name, int duration) : name(std::move(name)), duration(duration), remaining(duration), state(STOPPED) {
    using namespace std;
    time_t localTime = time(nullptr);
    tm* currentTime = localtime(&localTime);
    startDate = Date(*currentTime);

    time_t endTime = time(nullptr) + duration;
    tm* timerEndTime = localtime(&endTime);
    endDate = Date(*timerEndTime);
}

void Timer::start() {
    state = RUNNING;
    remaining = duration;
    auto start = std::chrono::high_resolution_clock::now();
    for(; remaining>0; --remaining)
    {
        if(state == RUNNING) {
            std::this_thread::sleep_until(start + (remaining+1)*std::chrono::seconds(1));
        }

        if(state == STOPPED)
            return;
    }
    stop();
}

void Timer::stop() {
    state = STOPPED;
    remaining = duration;
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

Date Timer::getStartDate() const {
    return startDate;
}

Date Timer::getEndDate() const {
    return endDate;
}

bool Timer::isRunning() const {
    return static_cast<bool>(state);
}


