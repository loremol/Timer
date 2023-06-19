#include <fstream>
#include <sstream>
#include "Frame.h"
#include "App.h"

wxBEGIN_EVENT_TABLE(frame, wxFrame)
                EVT_BUTTON(StartButton, frame::onStart)
                EVT_BUTTON(StopButton, frame::onStop)
                EVT_BUTTON(NewButton, frame::onNew)
                EVT_BUTTON(DeleteButton, frame::onDelete)
                EVT_BUTTON(RenameButton, frame::onRename)
                EVT_LISTBOX(ListBox, frame::onNewTimerSelection)
                EVT_SPINCTRL(SpinCtrlId, frame::updateCurrentTimerDuration)
wxEND_EVENT_TABLE()

frame::frame(const std::string &title) : wxFrame(nullptr, wxID_ANY, title),
                                         mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {
    allocateUiMemory();
    loadTimers();
    updateSpinCtrlValues();
    updateRemainingTime();
    updateNameField();
    updateControls();
    setupUi();
}

void frame::allocateUiMemory() {
    try {
        columns = new wxBoxSizer(wxHORIZONTAL);
        leftColumn = new wxBoxSizer(wxVERTICAL);
        rightColumn = new wxBoxSizer(wxVERTICAL);
        name = new wxBoxSizer(wxHORIZONTAL);
        remainingTime = new wxBoxSizer(wxHORIZONTAL);
        parameters = new wxBoxSizer(wxHORIZONTAL);
        controls = new wxBoxSizer(wxHORIZONTAL);
        yearsHoursParameters = new wxBoxSizer(wxVERTICAL);
        weeksMinutesParameters = new wxBoxSizer(wxVERTICAL);
        daysSecondsParameters = new wxBoxSizer(wxVERTICAL);
        hourPar = new wxBoxSizer(wxVERTICAL);
        minutePar = new wxBoxSizer(wxVERTICAL);
        secondPar = new wxBoxSizer(wxVERTICAL);
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, "Saved Timers");
        timerListBox = new wxListBox(mainPanel, ListBox, wxDefaultPosition, wxSize(200, 300), savedTimers);
        newButton = new wxButton(mainPanel, NewButton, "New");
        deleteButton = new wxButton(mainPanel, DeleteButton, "Delete");
        timerNameField = new wxTextCtrl(mainPanel, NameField, "");
        renameButton = new wxButton(mainPanel, RenameButton, "Rename");

        yearsLabel = new wxStaticText(mainPanel, wxID_ANY, "years");
        weeksLabel = new wxStaticText(mainPanel, wxID_ANY, "weeks");
        daysLabel = new wxStaticText(mainPanel, wxID_ANY, "days");
        hoursLabel = new wxStaticText(mainPanel, wxID_ANY, "hours");
        minutesLabel = new wxStaticText(mainPanel, wxID_ANY, "minutes");
        secondsLabel = new wxStaticText(mainPanel, wxID_ANY, "seconds");

        yearsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        weeksSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        daysSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        hoursSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        minutesSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        secondsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));

        startButton = new wxButton(mainPanel, StartButton, "Start");
        stopButton = new wxButton(mainPanel, StopButton, "Stop");
        remainingTimeStaticText = new wxStaticText(mainPanel, wxID_ANY, wxString(""));
        startDateText = new wxStaticText(mainPanel, wxID_ANY, "");
        stopDateText = new wxStaticText(mainPanel, wxID_ANY, "");
    } catch (const std::bad_alloc &e) {
        std::cerr << "Failed to allocate memory for the UI elements. Terminating." << std::endl;
        std::terminate();
    }
}

void frame::loadTimers() {
    for (auto &timer: timers)
        timerListBox->Append(timer->getName());

    if (!timers.empty()) {
        timerListBox->Select(0);
        currentTimer = timers[0];
    }
}

void frame::setupUi() {
    columns->Add(leftColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
    columns->Add(rightColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
    rightColumn->Add(name, wxSizerFlags().Center());
    rightColumn->Add(remainingTime, wxSizerFlags().Border(wxUP, 20).Center());
    rightColumn->Add(parameters, wxSizerFlags().Border(wxUP, 20).Center());
    rightColumn->Add(controls, wxSizerFlags().Border(wxUP, 20).Center());
    parameters->Add(yearsHoursParameters, wxSizerFlags().Center());
    parameters->Add(weeksMinutesParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(daysSecondsParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(hourPar, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(minutePar, wxSizerFlags().Border(wxLEFT, 5).Center());
    parameters->Add(secondPar, wxSizerFlags().Border(wxLEFT, 5).Center());

    leftColumn->Add(deleteButton, wxSizerFlags().Center());
    leftColumn->Add(timerListStaticText, wxSizerFlags().Center().Border(wxUP, 20));
    leftColumn->Add(timerListBox);

    name->Add(newButton, wxSizerFlags(wxALIGN_CENTER));
    name->Add(timerNameField,
              wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));
    name->Add(renameButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

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

    controls->Add(startButton, wxSizerFlags(wxALIGN_CENTER));
    controls->Add(stopButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

    rightColumn->Add(startDateText, wxSizerFlags().Border(wxUP, 15));
    rightColumn->Add(stopDateText, wxSizerFlags().Border(wxUP, 10));

    mainPanel->SetSizerAndFit(columns);
}

void frame::onStart(wxCommandEvent &event) {
    if (currentTimer->getState() == Stopped) {
        const auto f = [this]() { currentTimer->start(updateView); };
        if (threads.find(timerListBox->GetSelection()) == threads.end()) {
            threads.insert(std::pair<int, std::thread>(timerListBox->GetSelection(), std::thread{f}));
        }
    }
}

void frame::onStop(wxCommandEvent &event) {
    if (currentTimer->getState() == Running) {
        currentTimer->stop(updateView);
        threads.at(timerListBox->GetSelection()).join();
        threads.erase(timerListBox->GetSelection());
    }
}

void frame::updateNameField() {
    if (!timers.empty()) {
        timerNameField->SetValue(wxString(currentTimer->getName()));
    } else {
        timerNameField->SetValue("");
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
        renameButton->Enable(false);
        deleteButton->Enable(false);
        return;
    }
    if (currentTimer->getState() == RUNNING) {
        startButton->Enable(false);
        stopButton->Enable(true);
    } else {
        startButton->Enable(true);
        stopButton->Enable(false);
    }
    renameButton->Enable(true);
    deleteButton->Enable(true);
}

void frame::onNew(wxCommandEvent &event) {
    try {
        int newIndex = static_cast<int>(timers.size());
        wxString newName = wxGetTextFromUser("Enter the name of the new timer:", "New Timer",
                                             "Timer " + std::to_string(timers.size() + 1));
        if (newName.empty())
            return;
        timers.emplace_back(new Timer(newName.ToStdString(), 60));
        timerListBox->Append(newName);
        timerListBox->Select(newIndex);
        onNewTimerSelection(event);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Failed to allocate memory for a new timer. Terminating." << std::endl;
        std::terminate();
    }
}

void frame::onDelete(wxCommandEvent &event) {
    int index = timerListBox->GetSelection();
    if (timers.empty() || index == wxNOT_FOUND)
        return;

    timers[index]->stop([]() {});
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

void frame::onRename(wxCommandEvent &event) {
    std::string newName = timerNameField->GetValue().ToStdString();
    auto it = std::find(timers.begin(), timers.end(), currentTimer);
    int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
    timerListBox->SetString(timerListBoxIndex, newName);
    currentTimer->setName(newName);
}

void frame::onNewTimerSelection(wxCommandEvent &event) {
    int index = timerListBox->GetSelection();
    currentTimer = timers[index];
    updateRemainingTime();
    updateSpinCtrlValues();
    updateNameField();
    updateControls();
    updateTimerDates();
    mainPanel->Layout();
}

void frame::updateCurrentTimerDuration(wxSpinEvent &event) {
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
        yearsSpinCtrl->Enable(false);
        weeksSpinCtrl->Enable(false);
        daysSpinCtrl->Enable(false);
        hoursSpinCtrl->Enable(false);
        minutesSpinCtrl->Enable(false);
        secondsSpinCtrl->Enable(false);
        return;
    } else if (timers.size() == 1) {
        yearsSpinCtrl->Enable(true);
        weeksSpinCtrl->Enable(true);
        daysSpinCtrl->Enable(true);
        hoursSpinCtrl->Enable(true);
        minutesSpinCtrl->Enable(true);
        secondsSpinCtrl->Enable(true);
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
        if (currentTimer->getState() == RUNNING) {
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

