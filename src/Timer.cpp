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
    long yc{0}, monc{0}, wc{0}, dc{0}, hc{0}, mc{0}, sc{0};

    auto yearsIndex = format.find("%y");
    if (yearsIndex != string::npos) {
        auto y = duration_cast<duration<long, std::ratio<31536000>>>(remaining);
        remaining -= y;
        yc = y.count();
        auto before = format.substr(0, yearsIndex);
        auto after = format.substr(yearsIndex + 2);
        format = before + to_string(yc) + "y" + after;
    }

    auto monthIndex = format.find("%m");
    if (monthIndex != string::npos) {
        auto mon = duration_cast<months>(remaining);
        remaining -= mon;
        monc = mon.count();
        auto before = format.substr(0, monthIndex);
        auto after = format.substr(monthIndex + 2);
        format = before + to_string(monc) + "mon" + after;
    }

    auto weeksIndex = format.find("%w");
    if (weeksIndex != string::npos) {
        auto w = duration_cast<weeks>(remaining);
        remaining -= w;
        wc = w.count();
        auto before = format.substr(0, weeksIndex);
        auto after = format.substr(weeksIndex + 2);
        format = before + to_string(wc) + "w" + after;
    }

    auto daysIndex = format.find("%d");
    if (daysIndex != string::npos) {
        auto d = duration_cast<days>(remaining);
        remaining -= d;
        dc = d.count();
        auto before = format.substr(0, daysIndex);
        auto after = format.substr(daysIndex + 2);
        format = before + to_string(dc) + "d" + after;
    }

    auto hoursIndex = format.find("%H");
    if (hoursIndex != string::npos) {
        auto h = duration_cast<hours>(remaining);
        remaining -= h;
        hc = h.count();
        auto before = format.substr(0, hoursIndex);
        auto after = format.substr(hoursIndex + 2);
        format = before + to_string(hc) + "h" + after;
    }

    auto minutesIndex = format.find("%M");
    if (minutesIndex != string::npos) {
        auto min = duration_cast<minutes>(remaining);
        remaining -= min;
        mc = min.count();
        auto before = format.substr(0, minutesIndex);
        auto after = format.substr(minutesIndex + 2);
        format = before + to_string(mc) + "m" + after;
    }

    auto secondsIndex = format.find("%S");
    if (secondsIndex != string::npos) {
        auto s = duration_cast<seconds>(remaining);
        remaining -= s;
        sc = s.count();
        auto before = format.substr(0, secondsIndex);
        auto after = format.substr(secondsIndex + 2);
        format = before + to_string(sc) + "s" + after;
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
