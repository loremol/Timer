#include "Controller.h"

controller::controller() : view(new frame("Timer", *this)) {
    loadTimers();
    int width = 650, height = 400;
    view->SetClientSize(width, height);
    view->SetMinClientSize(wxSize(width, height));
    view->SetMaxClientSize(wxSize(width, height));
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
        timers.emplace_back(std::make_shared<timer>(newName.ToStdString(), 60, this));
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
    std::remove("timers.txt");
    std::ofstream file;
    file.open("timers.txt", std::ios_base::app);
    if (file.is_open()) {
        for (auto &timer: timers) {
            timer->saveToFile(file);
            timer->requestStop();
        }
    }
    file.close();
    for (auto &thread: threads)
        eraseTimerThread(thread.second.get_id());

    assert(threads.empty());
    view->Destroy();
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
    view->mainPanel->Layout();
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
        view->remainingTime().SetLabel(wxString(selectedTimer->getRemainingString("HH:MM:SS")));
    } else {
        view->remainingTime().SetLabel(wxString(""));
    }
}

void controller::updateTimerDates() {
    if (!timers.empty() && selectedTimer->isRunning()) {
        view->startDate().SetLabel(wxString("Timer started: " + selectedTimer->getStartDate().getFormatted()));
        view->endDate().SetLabel(wxString("Timer will stop: " + selectedTimer->getEndDate().getFormatted()));
    } else {
        view->startDate().SetLabel(wxString(""));
        view->endDate().SetLabel(wxString(""));
    }
}

void controller::updateSelectedTimerDuration() {
    int secondsQuantities[] = {3600 * 24 * 365, 3600 * 24 * 7, 3600 * 24, 3600, 60, 1};
    int i = 0, total = 0;
    for (auto &ctrl: view->timeControls()) {
        total += ctrl->GetValue() * secondsQuantities[i++];
    }
    selectedTimer->setDuration(total);
    updateRemainingTime();
    layoutView();
}

void controller::loadTimers() {
    std::ifstream file{};
    std::stringstream lineStream;
    std::string lineString, buffer, sState, sName, sStartDate, sEndDate;
    std::vector<std::string> line;
    file.open("timers.txt");
    if (file.is_open()) {
        while (std::getline(file, lineString)) {
            lineStream.str(lineString);
            while (std::getline(lineStream, buffer, '\t')) {
                line.push_back(buffer);
            }
            auto state = static_cast<std::atomic<bool>>(std::stoi(line[0]));
            sName = line[1];
            auto startTimestamp = static_cast<time_t>(std::stoi(line[2]));
            auto endTimestamp = static_cast<time_t>(std::stoi(line[3]));
            time_t currentTime = time(nullptr);
            if (currentTime > endTimestamp)
                state = Stopped;
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

    int duration = selectedTimer->getDuration();
    int years = std::floor(static_cast<float>(duration) / 31'556'952.f);
    int weeks = std::floor((duration - (years * 31'556'952)) / 604'800);
    int days = std::floor((duration - (years * 31'556'952) - (weeks * 604'800)) / 86'400);
    int hours = std::floor((duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400)) / 3600);
    int minutes = std::floor(
            (duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400) - (hours * 3600)) / 60);
    int seconds =
            duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400) - (hours * 3600) - (minutes * 60);
    std::vector<int> newParameters;
    newParameters.push_back(years);
    newParameters.push_back(weeks);
    newParameters.push_back(days);
    newParameters.push_back(hours);
    newParameters.push_back(minutes);
    newParameters.push_back(seconds);
    for (int i = 0; i < view->timeControls().size(); i++) {
        view->timeControls()[i]->SetValue(newParameters[i]);
    }
}

void controller::updateNameField() {
    if (!timers.empty()) {
        view->name().SetLabel(wxString(selectedTimer->getName()));
    } else {
        view->name().SetLabel("");
    }
}

void controller::waitForTimerStop(const std::shared_ptr<timer> &timer) {
    if (timer->isRunning()) {
        timer->requestStop();
        while (timer->isRunning()) {

        }
    }
}
