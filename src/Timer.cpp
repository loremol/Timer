#include <cmath>
#include <utility>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "Timer.h"
#include "App.h"

wxDECLARE_APP(app);

using namespace std::chrono;

timer::timer(std::string name, int duration, observer *controller)
        : name(std::move(name)), timerDuration(duration * 1000), remainingTime(timerDuration),
          state(Stopped), controller(controller) {
    checkConstructorParameters();
    calcStartEndDates();
}

timer::timer(std::string name, const bool &state,
             const time_point<system_clock, milliseconds> &startPoint,
             const time_point<system_clock, milliseconds> &endPoint,
             observer *controller) : name(std::move(name)), state(state), controller(controller),
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
    if (!isRunning()) {
        calcStartEndDates();
    }
    state = Running;
    wxGetApp().CallAfter([this]() {
        controller->updateControls();
        controller->updateTimerDates();
    });
    auto end = endDate.getPoint();
    auto lastUpdate = time_point_cast<milliseconds>(system_clock::now());
    while (lastUpdate < end) {
        if (stopRequested) break;
        lastUpdate = time_point_cast<milliseconds>(system_clock::now());
        remainingTime = end - lastUpdate;
        updateWhileRunning();
        std::this_thread::sleep_for(milliseconds(25));
    }
    stop();
}

void timer::stop() {
    stopRequested = false;
    state = Stopped;
    remainingTime = timerDuration;
    updateWhenFinished();
}

void timer::requestStop() {
    if (isRunning()) {
        finishedStopping = false;
        stopRequested = true;
    }
}

void timer::updateWhileRunning() {
    if (controller != nullptr)
        wxGetApp().CallAfter([this]() {
            controller->updateRemainingTime();
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
        finishedStopping = true;
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

std::string timer::formatRemainingTime(std::string format) {
    using namespace std;
    using namespace std::chrono;
    auto remaining = remainingTime;
    long yearCount{0}, monthCount{0}, weekCount{0}, dayCount{0}, hourCount{0}, minuteCount{0}, secondCount{0};

    auto yearsIndex = format.find("%y");
    if (yearsIndex != string::npos) {
        auto inYears = duration_cast<duration<long, std::ratio<31536000>>>(remaining);
        remaining -= inYears;
        yearCount = inYears.count();
        auto before = format.substr(0, yearsIndex);
        auto after = format.substr(yearsIndex + 2);
        format = before + to_string(yearCount) + "inYears" + after;
    }

    auto monthIndex = format.find("%m");
    if (monthIndex != string::npos) {
        auto inMonths = duration_cast<months>(remaining);
        remaining -= inMonths;
        monthCount = inMonths.count();
        auto before = format.substr(0, monthIndex);
        auto after = format.substr(monthIndex + 2);
        format = before + to_string(monthCount) + "inMonths" + after;
    }

    auto weeksIndex = format.find("%w");
    if (weeksIndex != string::npos) {
        auto inWeeks = duration_cast<weeks>(remaining);
        remaining -= inWeeks;
        weekCount = inWeeks.count();
        auto before = format.substr(0, weeksIndex);
        auto after = format.substr(weeksIndex + 2);
        format = before + to_string(weekCount) + "inWeeks" + after;
    }

    auto daysIndex = format.find("%d");
    if (daysIndex != string::npos) {
        auto inDays = duration_cast<days>(remaining);
        remaining -= inDays;
        dayCount = inDays.count();
        auto before = format.substr(0, daysIndex);
        auto after = format.substr(daysIndex + 2);
        format = before + to_string(dayCount) + "inDays" + after;
    }

    auto hoursIndex = format.find("%H");
    if (hoursIndex != string::npos) {
        auto inHours = duration_cast<hours>(remaining);
        remaining -= inHours;
        hourCount = inHours.count();
        auto before = format.substr(0, hoursIndex);
        auto after = format.substr(hoursIndex + 2);
        format = before + to_string(hourCount) + "inHours" + after;
    }

    auto minutesIndex = format.find("%M");
    if (minutesIndex != string::npos) {
        auto inMinutes = duration_cast<minutes>(remaining);
        remaining -= inMinutes;
        minuteCount = inMinutes.count();
        auto before = format.substr(0, minutesIndex);
        auto after = format.substr(minutesIndex + 2);
        format = before + to_string(minuteCount) + "m" + after;
    }

    auto secondsIndex = format.find("%S");
    if (secondsIndex != string::npos) {
        auto inSeconds = duration_cast<seconds>(remaining);
        remaining -= inSeconds;
        secondCount = inSeconds.count();
        auto before = format.substr(0, secondsIndex);
        auto after = format.substr(secondsIndex + 2);
        format = before + to_string(secondCount) + "inSeconds" + after;
    }

    return format;
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

long timer::getDuration() const {
    return static_cast<int>(duration_cast<seconds>(timerDuration).count());
}

bool timer::isRunning() const {
    return state;
}

bool timer::hasFinished() const {
    return finishedStopping;
}
