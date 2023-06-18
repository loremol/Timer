#include <fstream>
#include <sstream>
#include "Frame.h"
#include "App.h"

wxBEGIN_EVENT_TABLE(frame, wxFrame)
                EVT_BUTTON(StartButton, frame::onStart)
                EVT_BUTTON(StopButton, frame::onStop)
                EVT_BUTTON(NewButton, frame::onNew)
                EVT_BUTTON(RenameButton, frame::onRename)
                EVT_LISTBOX(ListBox, frame::onSelectCurrentTimer)
                EVT_SPINCTRL(SpinCtrlId, frame::updateCurrentTimerDuration)
wxEND_EVENT_TABLE()

frame::frame(const std::string &title) : wxFrame(nullptr, wxID_ANY, title),
                                         mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {
    initUi();
    loadTimers();
    setupUi();
    updateSpinCtrlValues();
    update();
}

void frame::initUi() {
    try {
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, "Saved Timers");
        timerListBox = new wxListBox(mainPanel, ListBox, wxDefaultPosition, wxSize(200, 300), savedTimers);
        newButton = new wxButton(mainPanel, NewButton, "New");
        timerNameField = new wxTextCtrl(mainPanel, NameField, "");
        renameButton = new wxButton(mainPanel, RenameButton, "Rename");
        yearsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        weeksSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        daysSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        hoursSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        minutesSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        secondsSpinCtrl = new wxSpinCtrl(mainPanel, SpinCtrlId, "", wxDefaultPosition, wxSize(120, -1));
        startButton = new wxButton(mainPanel, StartButton, "Start");
        stopButton = new wxButton(mainPanel, StopButton, "Stop");
        startDateText = new wxStaticText(mainPanel, wxID_ANY, "");
        stopDateText = new wxStaticText(mainPanel, wxID_ANY, "");
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for the UI elements. Terminating." << std::endl;
        std::terminate();
    }
}

void frame::loadTimers() {
    auto *t = new Timer("Timer1", 60);
    timers.push_back(t);
    if(timerListBox != nullptr)
        for (auto &timer: timers)
                timerListBox->Append(timer->getName());

    timerListBox->Select(0);
    currentTimer = timers[0];
}

void frame::setupUi() {
    try {
        auto *columns = new wxBoxSizer(wxHORIZONTAL);
        auto *leftColumn = new wxBoxSizer(wxVERTICAL);
        auto *rightColumn = new wxBoxSizer(wxVERTICAL);
        auto *name = new wxBoxSizer(wxHORIZONTAL);
        auto *parameters = new wxBoxSizer(wxHORIZONTAL);
        auto *controls = new wxBoxSizer(wxHORIZONTAL);
        auto *yearPar = new wxBoxSizer(wxVERTICAL);
        auto *monthPar = new wxBoxSizer(wxVERTICAL);
        auto *dayPar = new wxBoxSizer(wxVERTICAL);
        auto *hourPar = new wxBoxSizer(wxVERTICAL);
        auto *minutePar = new wxBoxSizer(wxVERTICAL);
        auto *secondPar = new wxBoxSizer(wxVERTICAL);
        columns->Add(leftColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
        columns->Add(rightColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
        rightColumn->Add(name, wxSizerFlags().Center());
        rightColumn->Add(parameters, wxSizerFlags().Border(wxUP, 20).Center());
        rightColumn->Add(controls, wxSizerFlags().Border(wxUP, 20).Center());
        parameters->Add(yearPar, wxSizerFlags().Center());
        parameters->Add(monthPar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(dayPar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(hourPar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(minutePar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(secondPar, wxSizerFlags().Border(wxLEFT, 5).Center());

        leftColumn->Add(timerListStaticText, wxSizerFlags().Border(wxUP, 20).Center());
        leftColumn->Add(timerListBox);

        name->Add(newButton, wxSizerFlags(wxALIGN_CENTER));
        name->Add(timerNameField,
                  wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));
        name->Add(renameButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

        yearPar->Add(new wxStaticText(mainPanel, wxID_ANY, "years"), wxSizerFlags().Center());
        monthPar->Add(new wxStaticText(mainPanel, wxID_ANY, "weeks"), wxSizerFlags().Center());
        dayPar->Add(new wxStaticText(mainPanel, wxID_ANY, "days"), wxSizerFlags().Center());

        yearPar->Add(yearsSpinCtrl);
        monthPar->Add(weeksSpinCtrl);
        dayPar->Add(daysSpinCtrl);
        yearPar->Add(new wxStaticText(mainPanel, wxID_ANY, "hours"), wxSizerFlags().Border(wxUP, 5).Center());
        yearPar->Add(hoursSpinCtrl);
        monthPar->Add(new wxStaticText(mainPanel, wxID_ANY, "minutes"), wxSizerFlags().Border(wxUP, 5).Center());
        monthPar->Add(minutesSpinCtrl);
        dayPar->Add(new wxStaticText(mainPanel, wxID_ANY, "seconds"), wxSizerFlags().Border(wxUP, 5).Center());
        dayPar->Add(secondsSpinCtrl);

        controls->Add(startButton, wxSizerFlags(wxALIGN_CENTER));
        controls->Add(stopButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

        remainingTimeStaticText = new wxStaticText(mainPanel, wxID_ANY, std::to_string(currentTimer->getRemaining()));
        remainingTimeStaticText->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        rightColumn->Add(remainingTimeStaticText, wxSizerFlags().Border(wxUP, 10).Center());

        rightColumn->Add(startDateText, wxSizerFlags().Border(wxUP, 10));
        rightColumn->Add(stopDateText, wxSizerFlags().Border(wxUP, 5));
        mainPanel->SetSizerAndFit(columns);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for the UI elements. Terminating." << std::endl;
        std::terminate();
    }
    update();
}

void frame::onStart(wxCommandEvent &event) {
    if (currentTimer->getState() == RUNNING)
        return;

    const auto f = [&]() { currentTimer->start(updateView); };
    thread = std::thread{f};
    thread.detach();
}

void frame::onStop(wxCommandEvent &event) {
    if (currentTimer->getState() == STOPPED)
        return;

    currentTimer->stop(updateView);
}

void frame::update() {
    remainingTimeStaticText->SetLabel(wxString(currentTimer->getRemainingString("HH:MM:SS")));
    timerNameField->SetValue(wxString(currentTimer->getName()));
    if (currentTimer->getState() == RUNNING) {
        startButton->Enable(false);
        stopButton->Enable(true);
        startDateText->SetLabel(wxString("Timer started: " + currentTimer->getStartDate().getFormatted()));
        stopDateText->SetLabel(wxString("Timer will stop: " + currentTimer->getEndDate().getFormatted()));
    } else {
        startButton->Enable(true);
        stopButton->Enable(false);
        startDateText->SetLabel(wxString(""));
        stopDateText->SetLabel(wxString(""));
    }
}

void frame::onNew(wxCommandEvent &event) {
    try {
        auto* newTimer = new Timer("Timer"+std::to_string(timers.size()+1),5);
        timers.push_back(newTimer);
        timerListBox->Append(newTimer->getName());
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for a new timer. Terminating." << std::endl;
        std::terminate();
    }

}

void frame::onRename(wxCommandEvent &event) {
    std::string newName = timerNameField->GetValue().ToStdString();
    auto it = std::find(timers.begin(), timers.end(), currentTimer);
    int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
    timerListBox->SetString(timerListBoxIndex, newName);
    currentTimer->setName(newName);
}

void frame::onSelectCurrentTimer(wxCommandEvent &event) {
    int selection = timerListBox->GetSelection();
    currentTimer = timers[selection];
    updateSpinCtrlValues();
    update();
}

void frame::updateCurrentTimerDuration(wxSpinEvent &event) {
    int total = secondsSpinCtrl->GetValue();
    total += minutesSpinCtrl->GetValue()*60;
    total += hoursSpinCtrl->GetValue()*3600;
    total += daysSpinCtrl->GetValue()*3600*24;
    total += weeksSpinCtrl->GetValue()*3600*24*7;
    total += yearsSpinCtrl->GetValue()*3600*24*365;
    currentTimer->setDuration(total);
    update();
}

void frame::updateSpinCtrlValues() {
    int duration = currentTimer->getDuration();
    int years = std::floor(static_cast<float>(duration) / 31'556'952.f);
    int weeks = std::floor((duration - (years*31'556'952)) / 604'800);
    int days = std::floor((duration - (years*31'556'952) - (weeks*604'800)) / 86'400);
    int hours = std::floor((duration - (years*31'556'952) - (weeks*604'800) - (days*86'400)) / 3600);
    int minutes = std::floor((duration - (years*31'556'952) - (weeks*604'800) - (days*86'400) - (hours*3600))/60);
    int seconds = duration - (years*31'556'952) - (weeks*604'800) - (days*86'400) - (hours*3600) - (minutes*60);
    yearsSpinCtrl->SetValue(years);
    weeksSpinCtrl->SetValue(weeks);
    daysSpinCtrl->SetValue(days);
    hoursSpinCtrl->SetValue(hours);
    minutesSpinCtrl->SetValue(minutes);
    secondsSpinCtrl->SetValue(seconds);
    update();
}

