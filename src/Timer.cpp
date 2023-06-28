#include "Timer.h"
#include "App.h"
#include <cmath>
#include <utility>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

wxDECLARE_APP(app);

timer::timer(std::string name, int duration, observer *controller)
        : name(std::move(name)), duration(duration), remaining(duration),
          state(Stopped), controller(controller) {
    calcStartEndDates();
}

timer::timer(std::string name, const bool &state, const time_t &startTimestamp, const time_t &endTimeStamp,
             observer *controller) : name(std::move(name)), state(state), controller(controller) {
    startDate = date(*localtime(&startTimestamp));
    endDate = date(*localtime(&endTimeStamp));
    duration = endTimeStamp - startTimestamp;
    if (isRunning()) {
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

void timer::start() {
    if (!isRunning()) {
        calcStartEndDates();
    }
    state = Running;
    controller->updateControls();
    using namespace std::chrono;
    auto end = endDate.getTimestamp();
    auto lastUpdate = round<seconds>(system_clock::now());
    while (lastUpdate < end) {
        if (stopRequested) break;
        lastUpdate = round<seconds>(system_clock::now());
        auto lastUpdateT = system_clock::to_time_t(lastUpdate);
        auto endT = system_clock::to_time_t(end);
        remaining = endT - lastUpdateT;
        updateWhileRunning();
        std::this_thread::sleep_for(milliseconds(25));
    }
    stop();
}

void timer::stop() {
    stopRequested = false;
    state = Stopped;
    remaining = duration;
    updateWhenFinished();
}

void timer::requestStop() {
    if (isRunning())
        stopRequested = true;
}

void timer::updateWhileRunning() {
    if (controller != nullptr)
        wxGetApp().CallAfter([this]() {
            controller->updateRemainingTime();
            controller->updateTimerDates();
            controller->layoutView();
        });
}

void timer::updateWhenFinished() {
    if (controller != nullptr) {
        wxGetApp().CallAfter([this]() {
            controller->updateRemainingTime();
            controller->updateControls();
            controller->updateTimerDates();
            controller->layoutView();
        });
        controller->eraseTimerThread(std::this_thread::get_id());
    }
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

bool timer::isRunning() const {
    return state;
}

