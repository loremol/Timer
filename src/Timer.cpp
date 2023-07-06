#include <utility>
#include <iostream>
#include <chrono>
#include <thread>
#include "Timer.h"

using namespace std::chrono;

timer::timer(std::string name, int duration, controllerInterface *controller)
        : name(std::move(name)), timerDuration(duration * 1000), remainingTime(timerDuration),
          state(Stopped), controller(controller) {
    checkConstructorParameters();
    calcStartEndDates();
}

timer::timer(std::string name, const bool &state,
             const time_point<system_clock, milliseconds> &startPoint,
             const time_point<system_clock, milliseconds> &endPoint,
             controllerInterface *controller) : name(std::move(name)), state(state), controller(controller),
                                                startDate(startPoint),
                                                endDate(endPoint) {
    checkConstructorParameters();
    timerDuration = endDate.getPoint() - startDate.getPoint();
    if (isRunning()) {
        auto now = system_clock::now();
        remainingTime = endDate.getPoint() - time_point_cast<milliseconds>(now);
        if (remainingTime < milliseconds(0)) {
            remainingTime = milliseconds(0);
            this->state = Stopped;
        }
    } else {
        remainingTime = timerDuration;
    }
}

void timer::checkConstructorParameters() {
    auto now = time_point_cast<seconds>(system_clock::now()).time_since_epoch().count();
    if (state == Running && endDate.getSecondsFromEpoch() <= now) state = Stopped;
}

void timer::calcStartEndDates() {
    auto nowMs = time_point_cast<milliseconds>(system_clock::now());
    startDate = date(nowMs);
    auto durationInMs = milliseconds{timerDuration};
    endDate = date(nowMs + durationInMs);
}

void timer::start() {
    if (!isRunning()) calcStartEndDates();
    state = Running;
    sendTimerStartedEvent();
    run();
}

void timer::run() {
    auto end = endDate.getPoint();
    auto lastUpdate = time_point_cast<milliseconds>(system_clock::now());
    while (lastUpdate < end) {
        if (stopRequested) break;
        lastUpdate = time_point_cast<milliseconds>(system_clock::now());
        remainingTime = end - lastUpdate;
        sendTimerTickEvent();
        std::this_thread::sleep_for(milliseconds(25));
    }
    stop();
}

void timer::stop() {
    stopRequested = false;
    remainingTime = timerDuration;
    sendTimerStoppedEvent();
    state = Stopped;
}

void timer::sendTimerStartedEvent() const {
    if (controller == nullptr) return;
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, TimerStarted);
    controller->getView()->GetEventHandler()->AddPendingEvent(event);
}

void timer::sendTimerTickEvent() const {
    if (controller == nullptr) return;
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, TimerTicked);
    controller->getView()->GetEventHandler()->AddPendingEvent(event);
}

void timer::sendTimerStoppedEvent() const {
    if (controller == nullptr) return;
    wxCommandEvent event(wxEVT_TEXT, TimerStopped);
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    event.SetString(ss.str());
    controller->getView()->GetEventHandler()->AddPendingEvent(event);
}

void timer::requestStop() {
    if (isRunning()) {
        stopRequested = true;
    }
}

void timer::setName(const std::string &s) {
    name = s;
}

void timer::setDuration(int newDuration) {
    timerDuration = duration_cast<milliseconds>(static_cast<seconds>(newDuration));
    if (state == Stopped)
        remainingTime = timerDuration;
    calcStartEndDates();
}

std::string timer::formatRemainingTime(std::string format) const {
    using namespace std;
    using namespace std::chrono;
    auto remaining = remainingTime;

    auto yearsIndex = format.find("%y");
    if (yearsIndex != string::npos) {
        auto inYears = duration_cast<duration<long, std::ratio<31536000>>>(remaining);
        remaining -= inYears;
        auto yearCount = inYears.count();
        auto before = format.substr(0, yearsIndex);
        auto after = format.substr(yearsIndex + 2);
        format = before + to_string(yearCount) + "y" + after;
    }

    auto monthIndex = format.find("%m");
    if (monthIndex != string::npos) {
        auto inMonths = duration_cast<months>(remaining);
        remaining -= inMonths;
        auto monthCount = inMonths.count();
        auto before = format.substr(0, monthIndex);
        auto after = format.substr(monthIndex + 2);
        format = before + to_string(monthCount) + "mo" + after;
    }

    auto weeksIndex = format.find("%w");
    if (weeksIndex != string::npos) {
        auto inWeeks = duration_cast<weeks>(remaining);
        remaining -= inWeeks;
        auto weekCount = inWeeks.count();
        auto before = format.substr(0, weeksIndex);
        auto after = format.substr(weeksIndex + 2);
        format = before + to_string(weekCount) + "w" + after;
    }

    auto daysIndex = format.find("%d");
    if (daysIndex != string::npos) {
        auto inDays = duration_cast<days>(remaining);
        remaining -= inDays;
        auto dayCount = inDays.count();
        auto before = format.substr(0, daysIndex);
        auto after = format.substr(daysIndex + 2);
        format = before + to_string(dayCount) + "d" + after;
    }

    auto hoursIndex = format.find("%H");
    if (hoursIndex != string::npos) {
        auto inHours = duration_cast<hours>(remaining);
        remaining -= inHours;
        auto hourCount = inHours.count();
        auto before = format.substr(0, hoursIndex);
        auto after = format.substr(hoursIndex + 2);
        format = before + to_string(hourCount) + "h" + after;
    }

    auto minutesIndex = format.find("%M");
    if (minutesIndex != string::npos) {
        auto inMinutes = duration_cast<minutes>(remaining);
        remaining -= inMinutes;
        auto minuteCount = inMinutes.count();
        auto before = format.substr(0, minutesIndex);
        auto after = format.substr(minutesIndex + 2);
        format = before + to_string(minuteCount) + "m" + after;
    }

    auto secondsIndex = format.find("%S");
    if (secondsIndex != string::npos) {
        auto inSeconds = duration_cast<seconds>(remaining);
        auto secondCount = inSeconds.count();
        auto before = format.substr(0, secondsIndex);
        auto after = format.substr(secondsIndex + 2);
        format = before + to_string(secondCount) + "s" + after;
    }

    return format;
}

date timer::getStartDate() const {
    return startDate;
}

date timer::getEndDate() const {
    return endDate;
}

const std::string &timer::getName() const {
    return name;
}

long timer::getDuration() const {
    return static_cast<int>(duration_cast<seconds>(timerDuration).count());
}

bool timer::isRunning() const {
    return state;
}
