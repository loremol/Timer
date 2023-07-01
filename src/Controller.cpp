#include "Controller.h"

float secondsQuantities[6] = {31536000.f, 604800.f, 86400.f, 3600.f, 60.f, 1.f};

controller::controller() : view(new frame("Timer", this)) {
    loadTimers();
    loadFormats();
    int width = 660, height = 400;
    view->SetClientSize(width, height);
    view->Center();
    view->Show();
    updateTimerView();
}

void controller::createNewTimer() {
    try {
        int newIndex = static_cast<int>(timers.size());
        std::string suggestedName = view->name().GetLabel().ToStdString();
        if (suggestedName.empty())
            suggestedName = "Timer " + std::to_string(timers.size() + 1);

        if (!timers.empty() || suggestedName.empty()) {
            if (selectedTimer->getName() == suggestedName)
                suggestedName = "Timer " + std::to_string(timers.size() + 1);
        }
        wxString newName = wxGetTextFromUser("Enter the new timer's name:", "New timer", suggestedName);
        if (newName.empty())
            return;
        timers.emplace_back(std::make_shared<timer>(newName.ToStdString(), 60 * 5, this));
        view->timerList().Append(newName);
        view->timerList().Select(newIndex);
        updateTimerView();
    } catch (const std::bad_alloc &e) {
        view->showMemoryError(false);
    }
}

void controller::deleteSelectedTimer() {
    int index = view->timerList().GetSelection();
    if (!timers.empty() && index != wxNOT_FOUND) {
        stopSelectedTimer();
        timers.erase(timers.begin() + index);

        view->timerList().Delete(index);
        if (index > 0) {
            view->timerList().Select(index - 1);
            selectedTimer = timers[index - 1];
        } else if (index == 0 && !timers.empty()) {
            index = 1;
            view->timerList().Select(index - 1);
            selectedTimer = timers[index - 1];
        }
        updateTimerView();
    }
}

void controller::renameSelectedTimer() {
    wxString newName = wxGetTextFromUser("Enter the new timer's name:", "Rename timer", selectedTimer->getName());
    if (!newName.empty()) {
        auto it = std::find(timers.begin(), timers.end(), selectedTimer);
        int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
        view->timerList().SetString(timerListBoxIndex, newName);
        selectedTimer->setName(newName.ToStdString());
        updateNameField();
    }
}

void controller::startSelectedTimer() {
    int runningTimers = 0;
    for (auto &timer: timers)
        if (timer->isRunning()) runningTimers++;
    if (threads.size() > runningTimers) return;

    const auto f = [this]() { selectedTimer->start(); };
    if (threads.find(view->timerList().GetSelection()) == threads.end()) {
        threads.insert(std::pair<int, std::thread>(view->timerList().GetSelection(), std::thread{f}));
    }
    updateTimerDates();
}

void controller::stopSelectedTimer() {
    if (!selectedTimer->isRunning()) return;
    waitForTimerStop(selectedTimer);
}

void controller::waitForTimerStop(const std::shared_ptr<timer> &timer) {
    if (timer->isRunning()) {
        timer->requestStop();
        while (!timer->hasFinished()) {

        }
    }
}

void controller::eraseTimerThread(const std::thread::id &threadId) {
    for (auto thread = threads.begin(); thread != threads.end(); thread++) {
        if (thread->second.get_id() == threadId) {
            int threadIndex = thread->first;
            if (threads.at(threadIndex).joinable()) {
                threads.at(threadIndex).detach();
                threads.erase(thread);
                break;
            }
        }
    }
}

void controller::closeWindow() {
    saveTimers();
    saveFormats();
    for (auto &thread: threads)
        eraseTimerThread(thread.second.get_id());

    assert(threads.empty());
    assert(timers.empty());
    view->Destroy();
}

void controller::loadTimers() {
    using namespace std::chrono;
    std::ifstream file{"timers.txt"};
    std::stringstream lineStream;
    std::string lineString, buffer, sState, sName, sStartDate, sEndDate;
    std::vector<std::string> line;
    if (file.is_open()) {
        while (std::getline(file, lineString)) {
            lineStream.str(lineString);
            while (std::getline(lineStream, buffer, '\t')) {
                line.push_back(buffer);
            }
            auto state = static_cast<std::atomic<bool>>(std::stoi(line.at(0)));
            sName = line.at(1);
            auto startTimestamp = time_point<system_clock, milliseconds>{milliseconds(std::stol(line.at(2)))};
            auto endTimestamp = time_point<system_clock, milliseconds>{milliseconds(std::stol(line.at(3)))};
            timers.emplace_back(std::make_shared<timer>(sName, state, startTimestamp, endTimestamp, this));

            line.clear();
            lineStream.clear();
        }
    }
    file.close();
    int i = 0;
    for (auto &timer: timers) {
        selectedTimer = timer;
        view->timerList().Append(timer->getName());
        view->timerList().SetSelection(i++);
        if (timer->isRunning())
            startSelectedTimer();
    }
    if (!timers.empty()) {
        view->timerList().Select(0);
        selectedTimer = timers[0];
    }
}

void controller::saveTimers() {
    remove("timers.txt");
    std::ofstream file{"timers.txt"};
    if (file.is_open()) {
        for (auto &timer: timers) {
            auto startDateMs = timer->getStartDate().getPoint().time_since_epoch().count();
            auto endDateMs = timer->getEndDate().getPoint().time_since_epoch().count();
            file << timer->isRunning() << "\t" << timer->getName() << "\t";
            file << startDateMs << "\t" << endDateMs << std::endl;
            timer->requestStop();
        }
    }
    timers.clear();
    file.close();
}

void controller::loadFormats() {
    std::ifstream file{"formats.txt"};
    std::string buffer;
    if (file.is_open()) {
        std::getline(file, buffer);
        dateFormat = buffer;
        std::getline(file, buffer);
        timerFormat = buffer;
    }
    file.close();
}

void controller::saveFormats() {
    remove("formats.txt");
    std::ofstream file{"formats.txt"};
    if (file.is_open()) {
        file << dateFormat << "\n" << timerFormat << "\n";
    }
    file.close();
}

void controller::updateTimerView() {
    int index = view->timerList().GetSelection();
    if (index != wxNOT_FOUND)
        selectedTimer = timers.at(index);

    updateSpinCtrlValues();
    updateRemainingTime();
    updateNameField();
    updateControls();
    updateTimerDates();
    layoutView();
}

void controller::layoutView() {
    view->panel()->Layout();
}

void controller::updateControls() {
    if (timers.empty()) {
        view->startButton->Enable(false);
        view->stopButton->Enable(false);
        view->renameBitmapButton->Enable(false);
        view->deleteBitmapButton->Enable(false);
        view->startButton->Show(false);
        view->stopButton->Show(false);
        return;
    }
    if (selectedTimer->isRunning()) {
        view->startButton->Enable(false);
        view->stopButton->Enable(true);
    } else {
        view->startButton->Enable(true);
        view->stopButton->Enable(false);
    }
    view->renameBitmapButton->Enable(true);
    view->deleteBitmapButton->Enable(true);
    view->startButton->Show(true);
    view->stopButton->Show(true);
}

void controller::updateRemainingTime() {
    if (!timers.empty() && selectedTimer != nullptr) {
        view->remainingTime().SetLabel(wxString(selectedTimer->formatRemainingTime(timerFormat)));
    } else {
        view->remainingTime().SetLabel(wxString(""));
    }
}

void controller::updateTimerDates() {
    if (!timers.empty() && selectedTimer->isRunning()) {
        view->startDate().SetLabel(wxString("Timer started: " + selectedTimer->getStartDate().formatDate(
                dateFormat)));
        view->endDate().SetLabel(wxString("Timer will stop: " + selectedTimer->getEndDate().formatDate(
                dateFormat)));
    } else {
        view->startDate().SetLabel(wxString(""));
        view->endDate().SetLabel(wxString(""));
    }
}

void controller::updateSelectedTimerDuration() {
    int i = 0, total = 0;
    for (auto &ctrl: view->timeControls()) {
        total += ctrl->GetValue() * static_cast<int>(secondsQuantities[i++]);
    }
    selectedTimer->setDuration(total);
    updateRemainingTime();
    layoutView();
}

void controller::updateSpinCtrlValues() {
    if (timers.empty()) {
        for (auto &spinCtrl: view->timeControls()) {
            spinCtrl->Enable(false);
            spinCtrl->Show(false);
        }
        for (auto &label: view->parameterTexts())
            label->Show(false);
        return;
    } else {
        for (auto &spinCtrl: view->timeControls()) {
            spinCtrl->Enable(true);
            spinCtrl->Show(true);
        }
        for (auto &label: view->parameterTexts())
            label->Show(true);
    }

    auto duration = static_cast<float>(selectedTimer->getDuration());
    float years = std::floor(static_cast<float>(duration) / secondsQuantities[0]);
    float weeks = std::floor((duration - (years * secondsQuantities[0])) / secondsQuantities[1]);
    float days = std::floor(
            (duration - (years * secondsQuantities[0]) - (weeks * secondsQuantities[1])) / secondsQuantities[2]);
    float hours = std::floor((duration - (years * secondsQuantities[0]) - (weeks * secondsQuantities[1]) -
                              (days * secondsQuantities[2])) / secondsQuantities[3]);
    float minutes = std::floor(
            (duration - (years * secondsQuantities[0]) - (weeks * secondsQuantities[1]) -
             (days * secondsQuantities[2]) - (hours * secondsQuantities[3])) / secondsQuantities[4]);
    float seconds = std::floor(
            duration - (years * secondsQuantities[0]) - (weeks * secondsQuantities[1]) - (days * secondsQuantities[2]) -
            (hours * secondsQuantities[3]) - (minutes * secondsQuantities[4]));

    std::vector<float> newParameters;
    newParameters.push_back(years);
    newParameters.push_back(weeks);
    newParameters.push_back(days);
    newParameters.push_back(hours);
    newParameters.push_back(minutes);
    newParameters.push_back(seconds);
    for (int i = 0; i < view->timeControls().size(); i++) {
        view->timeControls()[i]->SetValue(static_cast<int>(newParameters[i]));
    }
}

void controller::updateNameField() {
    if (!timers.empty()) {
        view->name().SetLabel(wxString(selectedTimer->getName()));
    } else {
        view->name().SetLabel(wxString(""));
    }
}

const std::string &controller::getTimerFormat() const {
    return timerFormat;
}

const std::string &controller::getDateFormat() const {
    return dateFormat;
}

void controller::changeTimerFormat(const std::string &newFormat) {
    this->timerFormat = newFormat;
}

void controller::changeDateFormat(const std::string &newFormat) {
    this->dateFormat = newFormat;
}
