#include "Frame.h"
#include "App.h"
#include "OptionsFrame.h"

wxBEGIN_EVENT_TABLE(frame, wxFrame)
                EVT_MENU(wxID_EXIT, frame::onMenuFileQuit)
                EVT_MENU(MenuEditOptions, frame::onMenuEditOptions)
                EVT_BUTTON(StartButton, frame::onStart)
                EVT_BUTTON(StopButton, frame::onStop)
                EVT_BUTTON(NewButton, frame::onNew)
                EVT_BUTTON(DeleteButton, frame::onDelete)
                EVT_BUTTON(RenameButton, frame::onRename)
                EVT_LISTBOX(ListBox, frame::onNewTimerSelection)
                EVT_SPINCTRL(SpinCtrlId, frame::updateCurrentTimerDuration)
                EVT_CLOSE(frame::onCloseWindow)
wxEND_EVENT_TABLE()

frame::frame(const std::string &title) : wxFrame(nullptr, wxID_ANY, title),
                                         mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {
    allocateUiMemory();
    loadTimers();
    updateSpinCtrlValues();
    updateRemainingTime();
    updateNameField();
    updateControls();
    updateSelection();
    setupUi();
}

void frame::allocateUiMemory() {
    try {
        menuBar = new wxMenuBar();
        fileMenu = new wxMenu();
        editMenu = new wxMenu();

        columns = new wxBoxSizer(wxHORIZONTAL);
        leftColumn = new wxBoxSizer(wxVERTICAL);
        rightColumn = new wxBoxSizer(wxVERTICAL);
        timerManagementButtons = new wxBoxSizer(wxHORIZONTAL);
        timerNameSizer = new wxBoxSizer(wxHORIZONTAL);
        remainingTime = new wxBoxSizer(wxHORIZONTAL);
        parameters = new wxBoxSizer(wxHORIZONTAL);
        timerStartStop = new wxBoxSizer(wxHORIZONTAL);
        yearsHoursParameters = new wxBoxSizer(wxVERTICAL);
        weeksMinutesParameters = new wxBoxSizer(wxVERTICAL);
        daysSecondsParameters = new wxBoxSizer(wxVERTICAL);
        hourPar = new wxBoxSizer(wxVERTICAL);
        minutePar = new wxBoxSizer(wxVERTICAL);
        secondPar = new wxBoxSizer(wxVERTICAL);
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, "Saved Timers");
        timerListBox = new wxListBox(mainPanel, ListBox, wxDefaultPosition, wxSize(200, 300), savedTimers);
        newBitmapButton = new wxBitmapButton(mainPanel, NewButton, wxBitmapBundle(wxIcon("plus.png")));
        deleteBitmapButton = new wxBitmapButton(mainPanel, DeleteButton, wxBitmapBundle(wxIcon("minus.png")));
        renameBitmapButton = new wxBitmapButton(mainPanel, RenameButton, wxBitmapBundle(wxIcon("rename.png")));

        timerNameField = new wxStaticText(mainPanel, NameField, "");

        yearsLabel = new wxStaticText(mainPanel, wxID_ANY, "Years");
        weeksLabel = new wxStaticText(mainPanel, wxID_ANY, "Weeks");
        daysLabel = new wxStaticText(mainPanel, wxID_ANY, "Days");
        hoursLabel = new wxStaticText(mainPanel, wxID_ANY, "Hours");
        minutesLabel = new wxStaticText(mainPanel, wxID_ANY, "Minutes");
        secondsLabel = new wxStaticText(mainPanel, wxID_ANY, "Seconds");
        parameterLabels.push_back(yearsLabel);
        parameterLabels.push_back(weeksLabel);
        parameterLabels.push_back(daysLabel);
        parameterLabels.push_back(hoursLabel);
        parameterLabels.push_back(minutesLabel);
        parameterLabels.push_back(secondsLabel);

        yearsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        weeksSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        daysSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        hoursSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        minutesSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        secondsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(118, -1));
        parameterControls.push_back(yearsSpinCtrl);
        parameterControls.push_back(weeksSpinCtrl);
        parameterControls.push_back(daysSpinCtrl);
        parameterControls.push_back(hoursSpinCtrl);
        parameterControls.push_back(minutesSpinCtrl);
        parameterControls.push_back(secondsSpinCtrl);

        startButton = new wxButton(mainPanel, StartButton, "Start");
        stopButton = new wxButton(mainPanel, StopButton, "Stop");
        remainingTimeStaticText = new wxStaticText(mainPanel, wxID_ANY, wxString(""));
        startDateText = new wxStaticText(mainPanel, wxID_ANY, "");
        stopDateText = new wxStaticText(mainPanel, wxID_ANY, "");
    } catch (const std::bad_alloc &e) {
        showMemoryError(true);
    }
}

void frame::showMemoryError(const bool &critical) {
    wxMessageDialog sd = wxMessageDialog(this, "Failed to allocate memory for the UI elements. Exiting the program.",
                                         "Memory error occurred", wxOK);
    sd.ShowModal();
    if (critical)
        exit(0);
}

void frame::loadTimers() {
    std::ifstream file{};
    std::stringstream lineStream;
    std::string lineString, buffer, sState, sName, sStartDate, sEndDate;
    std::vector<std::string> line;
    int timerCount = 0;
    file.open("timers.txt");
    if (file.is_open()) {
        while (std::getline(file, lineString)) {
            lineStream.str(lineString);
            while (std::getline(lineStream, buffer,'\t')) {
                line.push_back(buffer);
            }
            auto state = static_cast<std::atomic<bool>>(std::stoi(line[0]));
            sName = line[1];
            auto startTimestamp = static_cast<time_t>(std::stoi(line[2]));
            auto endTimestamp = static_cast<time_t>(std::stoi(line[3]));
            time_t currentTime = time(nullptr);
            if (currentTime > endTimestamp)
                state = Stopped;
            timers.emplace_back(std::make_shared<timer>(sName, state, startTimestamp, endTimestamp));
            currentTimer = timers[timerCount];
            timerListBox->Append(currentTimer->getName());
            timerListBox->SetSelection(timerCount);
            if (currentTimer->isRunning()) {
                startCurrentTimer();
            }
            line.clear();
            lineStream.clear();
            timerCount++;
        }
    }
    file.close();
    if (!timers.empty()) {
        timerListBox->Select(0);
        currentTimer = timers[0];
    }
}

void frame::setupUi() {
    fileMenu->Append(wxID_EXIT, _T("&Quit"));
    editMenu->Append(MenuEditOptions, _T("&Options"));
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(editMenu, _T("&Edit"));

    columns->Add(leftColumn, wxSizerFlags().Border(wxLEFT, 25));
    columns->Add(rightColumn, wxSizerFlags().Border(wxLEFT, 30));

    leftColumn->AddSpacer(10);
    leftColumn->Add(timerListStaticText, wxSizerFlags().Center());
    leftColumn->Add(timerListBox, wxSizerFlags().Border(wxUP, 5));
    leftColumn->Add(timerManagementButtons, wxSizerFlags().Center().Border(wxUP, 7));

    timerManagementButtons->Add(newBitmapButton, wxSizerFlags().Center());
    timerManagementButtons->Add(deleteBitmapButton, wxSizerFlags().Center().Border(wxLEFT, 7));
    timerManagementButtons->Add(renameBitmapButton, wxSizerFlags().Center().Border(wxLEFT, 7));

    rightColumn->AddSpacer(10);
    rightColumn->Add(timerNameSizer, wxSizerFlags().Center());
    rightColumn->Add(remainingTime, wxSizerFlags().Border(wxUP, 5).Center());
    rightColumn->Add(parameters, wxSizerFlags().Border(wxUP, 5).Center());
    rightColumn->Add(timerStartStop, wxSizerFlags().Border(wxUP, 15).Center());

    parameters->Add(yearsHoursParameters, wxSizerFlags().Center());
    parameters->Add(weeksMinutesParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(daysSecondsParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(hourPar, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(minutePar, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(secondPar, wxSizerFlags().Border(wxLEFT, 5).Center());

    timerNameSizer->Add(timerNameField, wxSizerFlags().Center());
    timerNameField->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    remainingTime->AddStretchSpacer();
    remainingTime->Add(remainingTimeStaticText, wxSizerFlags(wxALIGN_CENTER).Border(wxUP, 10));
    remainingTime->AddStretchSpacer();
    remainingTimeStaticText->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    yearsHoursParameters->Add(yearsLabel, wxSizerFlags().Center());
    yearsHoursParameters->Add(yearsSpinCtrl);
    yearsHoursParameters->Add(hoursLabel, wxSizerFlags().Border(wxUP, 5).Center());
    yearsHoursParameters->Add(hoursSpinCtrl);

    weeksMinutesParameters->Add(weeksLabel, wxSizerFlags().Center());
    weeksMinutesParameters->Add(weeksSpinCtrl);
    weeksMinutesParameters->Add(minutesLabel, wxSizerFlags().Border(wxUP, 5).Center());
    weeksMinutesParameters->Add(minutesSpinCtrl);

    daysSecondsParameters->Add(daysLabel, wxSizerFlags().Center());
    daysSecondsParameters->Add(daysSpinCtrl);
    daysSecondsParameters->Add(secondsLabel, wxSizerFlags().Border(wxUP, 5).Center());
    daysSecondsParameters->Add(secondsSpinCtrl);

    timerStartStop->Add(startButton, wxSizerFlags(wxALIGN_CENTER));
    timerStartStop->Add(stopButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

    rightColumn->Add(startDateText, wxSizerFlags().Border(wxUP, 15));
    rightColumn->Add(stopDateText, wxSizerFlags().Border(wxUP, 10));

    SetMenuBar(menuBar);
    mainPanel->SetSizerAndFit(columns);
}

void frame::onCloseWindow(wxCloseEvent &WXUNUSED(event)) {
    remove("timers.txt");
    std::ofstream file;
    file.open("timers.txt", std::ios_base::app);
    if (file.is_open()) {
        for (auto &timer: timers) {
            timer->saveToFile(file);
            timer->requestStop();
        }
    }
    file.close();
    bool allThreadsStopped = false;
    while (!allThreadsStopped) {
        allThreadsStopped = true;
        for (auto &timer: timers) {
            if (timer->isRunning())
                allThreadsStopped = false;
        }
    }
    for (auto &thread: threads) {
        if (thread.second.joinable()) {
            thread.second.join();
        }
    }
    wxGetApp().CallAfter([this]() { Destroy(); });
}

void frame::onStart(wxCommandEvent &WXUNUSED(event)) {
    startCurrentTimer();
}

void frame::startCurrentTimer() {
    const auto f = [this]() { currentTimer->start(updateView); };
    if (threads.find(timerListBox->GetSelection()) == threads.end()) {
        threads.insert(std::pair<int, std::thread>(timerListBox->GetSelection(), std::thread{f}));
    }
}

void frame::onStop(wxCommandEvent &event) {
    if (currentTimer->isRunning()) {
        currentTimer->requestStop();
        while (currentTimer->isRunning()) {

        }
        if (threads.at(timerListBox->GetSelection()).joinable()) {
            threads.at(timerListBox->GetSelection()).join();
            threads.erase(timerListBox->GetSelection());
        }
    }
}

void frame::updateNameField() {
    if (!timers.empty()) {
        timerNameField->SetLabel(wxString(currentTimer->getName()));
    } else {
        timerNameField->SetLabel("");
    }
}

void frame::updateRemainingTime() {
    if (!timers.empty() && currentTimer != nullptr) {
        remainingTimeStaticText->SetLabel(wxString(currentTimer->getRemainingString("HH:MM:SS")));
    } else {
        remainingTimeStaticText->SetLabel(wxString(""));
    }
}

void frame::updateControls() {
    if (timers.empty()) {
        startButton->Enable(false);
        stopButton->Enable(false);
        renameBitmapButton->Enable(false);
        deleteBitmapButton->Enable(false);
        startButton->Show(false);
        stopButton->Show(false);
        return;
    }
    if (currentTimer->isRunning()) {
        startButton->Enable(false);
        stopButton->Enable(true);
    } else {
        startButton->Enable(true);
        stopButton->Enable(false);
    }
    renameBitmapButton->Enable(true);
    deleteBitmapButton->Enable(true);
    startButton->Show(true);
    stopButton->Show(true);
}

void frame::updateCurrentTimerDuration(wxSpinEvent &WXUNUSED(event)) {
    int total = secondsSpinCtrl->GetValue();
    total += minutesSpinCtrl->GetValue() * 60;
    total += hoursSpinCtrl->GetValue() * 3600;
    total += daysSpinCtrl->GetValue() * 3600 * 24;
    total += weeksSpinCtrl->GetValue() * 3600 * 24 * 7;
    total += yearsSpinCtrl->GetValue() * 3600 * 24 * 365;
    currentTimer->setDuration(total);
    updateRemainingTime();
    mainPanel->Layout();
}

void frame::updateSpinCtrlValues() {
    if (timers.empty()) {
        for (auto &spinCtrl: parameterControls) {
            spinCtrl->Enable(false);
            spinCtrl->Show(false);
        }
        for (auto &label: parameterLabels)
            label->Show(false);
        return;
    } else if (timers.size() == 1) {
        for (auto &spinCtrl: parameterControls) {
            spinCtrl->Enable(true);
            spinCtrl->Show(true);
        }
        for (auto &label: parameterLabels)
            label->Show(true);
    }
    int duration = currentTimer->getDuration();
    int years = std::floor(static_cast<float>(duration) / 31'556'952.f);
    int weeks = std::floor((duration - (years * 31'556'952)) / 604'800);
    int days = std::floor((duration - (years * 31'556'952) - (weeks * 604'800)) / 86'400);
    int hours = std::floor((duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400)) / 3600);
    int minutes = std::floor(
            (duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400) - (hours * 3600)) / 60);
    int seconds =
            duration - (years * 31'556'952) - (weeks * 604'800) - (days * 86'400) - (hours * 3600) - (minutes * 60);
    yearsSpinCtrl->SetValue(years);
    weeksSpinCtrl->SetValue(weeks);
    daysSpinCtrl->SetValue(days);
    hoursSpinCtrl->SetValue(hours);
    minutesSpinCtrl->SetValue(minutes);
    secondsSpinCtrl->SetValue(seconds);
}

void frame::updateTimerDates() {
    if (!timers.empty()) {
        if (currentTimer->isRunning()) {
            startDateText->SetLabel(wxString("Timer started: " + currentTimer->getStartDate().getFormatted()));
            stopDateText->SetLabel(wxString("Timer will stop: " + currentTimer->getEndDate().getFormatted()));
        } else {
            startDateText->SetLabel(wxString(""));
            stopDateText->SetLabel(wxString(""));
        }
    } else {
        startDateText->SetLabel(wxString(""));
        stopDateText->SetLabel(wxString(""));
    }
}

void frame::onMenuFileQuit(wxCommandEvent &WXUNUSED(event)) {
    Close(false);
}

void frame::onMenuEditOptions(wxCommandEvent &WXUNUSED(event)) {
    auto *options = new optionsFrame("Options");
    int width = 550, height = 350;
    options->SetClientSize(width, height);
    options->SetMinClientSize(wxSize(width, height));
    options->SetMaxClientSize(wxSize(width, height));
    options->Center();
    options->ShowModal();
}

void frame::onNew(wxCommandEvent &event) {
    try {
        int newIndex = static_cast<int>(timers.size());
        std::string suggestedName = timerNameField->GetLabel().ToStdString();
        if (suggestedName.empty())
            suggestedName = "Timer " + std::to_string(timers.size() + 1);

        if (!timers.empty() || suggestedName.empty()) {
            if (currentTimer->getName() == suggestedName)
                suggestedName = "Timer " + std::to_string(timers.size() + 1);
        }
        wxString newName = wxGetTextFromUser("Enter the new timer's name:", "New timer", suggestedName);
        if (newName.empty())
            return;
        timers.emplace_back(std::make_shared<timer>(newName.ToStdString(), 60));
        timerListBox->Append(newName);
        timerListBox->Select(newIndex);
        onNewTimerSelection(event);
    } catch (const std::bad_alloc &e) {
        showMemoryError(false);
    }
}

void frame::onDelete(wxCommandEvent &event) {
    int index = timerListBox->GetSelection();
    if (timers.empty() || index == wxNOT_FOUND)
        return;

    onStop(event);
    auto deletedTimer = timers.begin() + index;
    timers.erase(deletedTimer);
    timerListBox->Delete(index);
    if (index > 0) {
        timerListBox->Select(index - 1);
        currentTimer = timers[index - 1];
    } else if (index == 0 && !timers.empty()) {
        index = 1;
        timerListBox->Select(index - 1);
        currentTimer = timers[index - 1];
    }

    onNewTimerSelection(event);
}

void frame::onRename(wxCommandEvent &WXUNUSED(event)) {
    wxString newName = wxGetTextFromUser("Enter the new timer's name:", "Rename timer", currentTimer->getName());
    if (!newName.empty()) {
        auto it = std::find(timers.begin(), timers.end(), currentTimer);
        int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
        timerListBox->SetString(timerListBoxIndex, newName);
        currentTimer->setName(newName.ToStdString());
        updateNameField();
    }
}

void frame::onNewTimerSelection(wxCommandEvent &WXUNUSED(event)) {
    updateSelection();
}

void frame::updateSelection() {
    int index = timerListBox->GetSelection();
    if (index != -1)
        currentTimer = timers[index];
    updateRemainingTime();
    updateSpinCtrlValues();
    updateNameField();
    updateControls();
    updateTimerDates();
    mainPanel->Layout();
}
