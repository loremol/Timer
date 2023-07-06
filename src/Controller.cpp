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
        std::string suggestedName = "Timer " + std::to_string(timers.size() + 1);
        if (!timers.empty()) {
            if (selectedTimer->getName() == suggestedName)
                suggestedName = "Timer " + std::to_string(timers.size() + 1);
        }
        wxString newName;
        while (newName.empty())
            newName = wxGetTextFromUser("Enter the new timer's name (it shouldn't be empty):", "New timer",
                                        suggestedName);

        int newIndex = static_cast<int>(timers.size());
        timers.emplace_back(std::make_shared<timer>(newName.ToStdString(), 60 * 5, this));
        view->timerList()->Append(newName);
        view->timerList()->Select(newIndex);
        updateTimerView();
    } catch (const std::bad_alloc &e) {
        view->showMemoryError();
    }
}

void controller::deleteSelectedTimer() {
    if (!timers.empty() && selectedTimer != nullptr) {
        if (selectedTimer->isRunning())
            stopSelectedTimer();
        auto timerToDelete = std::find_if(timers.begin(), timers.end(),
                                          [this](const std::shared_ptr<timer> &timer) -> bool {
                                              return selectedTimer == timer;
                                          });
        timers.erase(timerToDelete);
        int index = static_cast<int>(std::distance(timers.begin(), timerToDelete));
        view->timerList()->Delete(index);
        view->timerList()->Select(index - 1);

        if (index > 0)
            selectedTimer = timers[index - 1];
        else
            selectedTimer = nullptr;

        updateTimerView();
    }
}

void controller::renameSelectedTimer() {
    wxString newName;
    while (newName.empty())
        newName = wxGetTextFromUser("Enter the new timer's name (it shouldn't be empty):", "Rename timer",
                                    selectedTimer->getName());
    auto it = std::find(timers.begin(), timers.end(), selectedTimer);
    int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
    view->timerList()->SetString(timerListBoxIndex, newName);
    selectedTimer->setName(newName.ToStdString());
    updateNameField();
}

void controller::startSelectedTimer() {
    auto runningTimers = std::count_if(timers.begin(), timers.end(), [](const std::shared_ptr<timer> &timer) -> bool {
        return timer->isRunning();
    });
    if (threads.size() > runningTimers) return;

    if (threads.find(view->timerList()->GetSelection()) == threads.end()) {
        int threadId = view->timerList()->GetSelection();
        std::thread newThread(&timer::start, selectedTimer);
        threads.emplace(std::pair<int, std::thread>(threadId, std::move(newThread)));
    }
    updateTimerDates();
}

void controller::stopSelectedTimer() {
    if (selectedTimer == nullptr) return;
    if (!selectedTimer->isRunning()) return;
    selectedTimer->requestStop();
    while (selectedTimer->isRunning()) {}
    wxYield();
    updateRemainingTime();
    updateControls();
    updateTimerDates();
    layoutView();
}

void controller::eraseTimerThread(const std::string &threadId) {
    auto threadToDelete = std::find_if(threads.begin(), threads.end(),
                                       [&threadId](const std::pair<const int, std::thread> &t) -> bool {
                                           return stringifyThreadId(t.second.get_id()) == threadId;
                                       });
    if (threadToDelete != threads.end() && threadToDelete->second.joinable()) {
        threadToDelete->second.join();
        threads.erase(threadToDelete);
    }
}

std::string controller::stringifyThreadId(const std::thread::id &threadId) {
    std::ostringstream ss;
    ss << threadId;
    return ss.str();
}

void controller::closeWindow() {
    saveAndClose();
}

void controller::saveAndClose() {
    saveTimers();
    saveFormats();

    for (const auto &timer: std::ranges::reverse_view(timers)) {
        selectedTimer = timer;
        deleteSelectedTimer();
    }

    assert(timers.empty());
    assert(threads.empty());
    view->Destroy();
}

void controller::loadTimers() {
    using namespace std::chrono;
    std::ifstream file{"timers.txt"};
    std::stringstream lineStream;
    std::vector<std::string> line;
    if (file.is_open()) {
        std::string lineString;
        while (std::getline(file, lineString)) {
            std::string buffer, sName;
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
    for (const auto &timer: timers) {
        selectedTimer = timer;
        view->timerList()->Append(timer->getName());
        view->timerList()->SetSelection(i++);
        if (timer->isRunning())
            startSelectedTimer();
    }
    if (!timers.empty()) {
        view->timerList()->Select(0);
        selectedTimer = timers[0];
    }
}

void controller::saveTimers() const {
    remove("timers.txt");
    std::ofstream file{"timers.txt"};
    if (file.is_open()) {
        for (const auto &timer: timers) {
            auto startDateMs = timer->getStartDate().getPoint().time_since_epoch().count();
            auto endDateMs = timer->getEndDate().getPoint().time_since_epoch().count();
            file << timer->isRunning() << "\t" << timer->getName() << "\t";
            file << startDateMs << "\t" << endDateMs << std::endl;
        }
    }
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

void controller::saveFormats() const {
    remove("formats.txt");
    std::ofstream file{"formats.txt"};
    if (file.is_open()) {
        file << dateFormat << "\n" << timerFormat << "\n";
    }
    file.close();
}

void controller::updateTimerView() {
    updateSelectedTimer();
    updateSpinCtrlValues();
    updateRemainingTime();
    updateNameField();
    updateControls();
    updateTimerDates();
    layoutView();
}

void controller::updateSelectedTimer() {
    int index = view->timerList()->GetSelection();
    if (index != wxNOT_FOUND)
        selectedTimer = timers.at(index);
}

void controller::layoutView() {
    view->panel()->Layout();
}

void controller::updateControls() {
    if (timers.empty() || selectedTimer == nullptr) {
        view->getStartButton()->Enable(false);
        view->getStopButton()->Enable(false);
        view->getRenameBitmapButton()->Enable(false);
        view->getDeleteBitmapButton()->Enable(false);
        view->getStartButton()->Show(false);
        view->getStopButton()->Show(false);
        return;
    }
    if (selectedTimer->isRunning()) {
        view->getStartButton()->Enable(false);
        view->getStopButton()->Enable(true);
    } else {
        view->getStartButton()->Enable(true);
        view->getStopButton()->Enable(false);
    }
    view->getRenameBitmapButton()->Enable(true);
    view->getDeleteBitmapButton()->Enable(true);
    view->getStartButton()->Show(true);
    view->getStopButton()->Show(true);
}

void controller::updateRemainingTime() {
    if (!timers.empty() && selectedTimer != nullptr) {
        view->remainingTime()->SetLabel(wxString(selectedTimer->formatRemainingTime(timerFormat)));
    } else {
        view->remainingTime()->SetLabel(wxString(""));
    }
}

void controller::updateTimerDates() {
    if (selectedTimer == nullptr || timers.empty() || !selectedTimer->isRunning()) {
        view->startDate()->SetLabel(wxString(""));
        view->endDate()->SetLabel(wxString(""));
    } else {
        view->startDate()->SetLabel(wxString("Timer started: " + selectedTimer->getStartDate().format(
                dateFormat)));
        view->endDate()->SetLabel(wxString("Timer will stop: " + selectedTimer->getEndDate().format(
                dateFormat)));
    }
}

void controller::updateSelectedTimerDuration() {
    int i = 0;
    auto total = std::accumulate(view->timeControls().begin(), view->timeControls().end(), 0,
                                 [&i](auto a, auto spinCtrl) -> int {
                                     return a + spinCtrl->GetValue() * static_cast<int>(secondsQuantities[i++]);
                                 });

    selectedTimer->setDuration(total);
    updateRemainingTime();
    layoutView();
}

void controller::updateSpinCtrlValues() {
    if (timers.empty() || selectedTimer == nullptr) {
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
    if (!timers.empty() && selectedTimer != nullptr) {
        view->name()->SetLabel(wxString(selectedTimer->getName()));
    } else {
        view->name()->SetLabel(wxString(""));
    }
}

void controller::changeTimerFormat(const std::string &newFormat) {
    timerFormat = newFormat;
}

void controller::changeDateFormat(const std::string &newFormat) {
    dateFormat = newFormat;
}

const std::string &controller::getTimerFormat() const {
    return timerFormat;
}

const std::string &controller::getDateFormat() const {
    return dateFormat;
}

wxFrame *controller::getView() const {
    return view;
}

const std::vector<std::shared_ptr<timer>> &controller::getTimers() const {
    return timers;
}

const std::map<int, std::thread> &controller::getThreads() const {
    return threads;
}

const std::shared_ptr<timer> &controller::getSelectedTimer() const {
    return selectedTimer;
}
