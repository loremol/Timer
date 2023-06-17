#include <fstream>
#include <sstream>
#include "MainFrame.h"
#include "App.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(STARTBUTTON, MainFrame::OnStart)
                EVT_BUTTON(STOPBUTTON, MainFrame::OnStop)
                EVT_BUTTON(NEWBUTTON, MainFrame::OnNew)
                EVT_BUTTON(RENAMEBUTTON, MainFrame::OnRename)
                EVT_LISTBOX(LISTBOX, MainFrame::OnSelectCurrentTimer)
                EVT_SPINCTRL(SPINCTRLID, MainFrame::updateCurrentTimerDuration)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const std::string &title) : wxFrame(nullptr, wxID_ANY, title),
                                                 mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {
    initUI();
    loadTimers();
    setupUI();
    updateSpinCtrlValues();
    update();
}

void MainFrame::initUI() {
    try {
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, "Saved Timers");
        timerListBox = new wxListBox(mainPanel, LISTBOX, wxDefaultPosition, wxSize(200, 300), savedTimers);
        newButton = new wxButton(mainPanel, NEWBUTTON, "New");
        timerNameField = new wxTextCtrl(mainPanel, NAMEFIELD, "");
        renameButton = new wxButton(mainPanel, RENAMEBUTTON, "Rename");
        yearsSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        weeksSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        daysSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        hoursSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        minutesSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        secondsSpinCtrl = new wxSpinCtrl(mainPanel, SPINCTRLID, "", wxDefaultPosition, wxSize(120, -1));
        startButton = new wxButton(mainPanel, STARTBUTTON, "Start");
        stopButton = new wxButton(mainPanel, STOPBUTTON, "Stop");
        startDateText = new wxStaticText(mainPanel, wxID_ANY, "");
        stopDateText = new wxStaticText(mainPanel, wxID_ANY, "");
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for the UI elements. Terminating." << std::endl;
        std::terminate();
    }
}

void MainFrame::loadTimers() {
    auto *t = new Timer("Timer1", 3665);
    auto *t2 = new Timer("Timer2", 5);
    timers.push_back(t);
    timers.push_back(t2);
    if(timerListBox != nullptr)
        for (auto &timer: timers)
                timerListBox->Append(timer->getName());

    timerListBox->Select(0);
    currentTimer = timers[0];
}

void MainFrame::setupUI() {
    try {
        auto *columns = new wxBoxSizer(wxHORIZONTAL);
        auto *leftColumn = new wxBoxSizer(wxVERTICAL);
        auto *rightColumn = new wxBoxSizer(wxVERTICAL);
        auto *name = new wxBoxSizer(wxHORIZONTAL);
        auto *parameters = new wxBoxSizer(wxHORIZONTAL);
        auto *controls = new wxBoxSizer(wxHORIZONTAL);
        auto *year_par = new wxBoxSizer(wxVERTICAL);
        auto *month_par = new wxBoxSizer(wxVERTICAL);
        auto *day_par = new wxBoxSizer(wxVERTICAL);
        auto *hour_par = new wxBoxSizer(wxVERTICAL);
        auto *minute_par = new wxBoxSizer(wxVERTICAL);
        auto *second_par = new wxBoxSizer(wxVERTICAL);
        columns->Add(leftColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
        columns->Add(rightColumn, wxSizerFlags().Border(wxUP | wxLEFT, 20));
        rightColumn->Add(name, wxSizerFlags().Center());
        rightColumn->Add(parameters, wxSizerFlags().Border(wxUP, 20).Center());
        rightColumn->Add(controls, wxSizerFlags().Border(wxUP, 20).Center());
        parameters->Add(year_par, wxSizerFlags().Center());
        parameters->Add(month_par, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(day_par, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(hour_par, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(minute_par, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(second_par, wxSizerFlags().Border(wxLEFT, 5).Center());

        leftColumn->Add(timerListStaticText, wxSizerFlags().Border(wxUP, 20).Center());
        leftColumn->Add(timerListBox);

        name->Add(newButton, wxSizerFlags(wxALIGN_CENTER));
        name->Add(timerNameField,
                  wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));
        name->Add(renameButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

        year_par->Add(new wxStaticText(mainPanel, wxID_ANY, "years"), wxSizerFlags().Center());
        month_par->Add(new wxStaticText(mainPanel, wxID_ANY, "weeks"), wxSizerFlags().Center());
        day_par->Add(new wxStaticText(mainPanel, wxID_ANY, "days"), wxSizerFlags().Center());

        year_par->Add(yearsSpinCtrl);
        month_par->Add(weeksSpinCtrl);
        day_par->Add(daysSpinCtrl);
        year_par->Add(new wxStaticText(mainPanel, wxID_ANY, "hours"), wxSizerFlags().Border(wxUP, 5).Center());
        year_par->Add(hoursSpinCtrl);
        month_par->Add(new wxStaticText(mainPanel, wxID_ANY, "minutes"), wxSizerFlags().Border(wxUP, 5).Center());
        month_par->Add(minutesSpinCtrl);
        day_par->Add(new wxStaticText(mainPanel, wxID_ANY, "seconds"), wxSizerFlags().Border(wxUP, 5).Center());
        day_par->Add(secondsSpinCtrl);

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
    setupEventHandling();
    update();
}

void MainFrame::setupEventHandling() {
    startButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnStart), nullptr, this);
    stopButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnStop), nullptr, this);
    newButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnNew), nullptr, this);
    renameButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnRename), nullptr, this);
}

void MainFrame::OnStart(wxCommandEvent &event) {
    if (currentTimer->getState() == RUNNING)
        return;

    const auto f = [&]() { currentTimer->start(updateView); };
    thread = std::thread{f};
    thread.detach();
}

void MainFrame::OnStop(wxCommandEvent &event) {
    if (currentTimer->getState() == STOPPED)
        return;

    currentTimer->stop(updateView);
}

void MainFrame::update() {
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

void MainFrame::OnNew(wxCommandEvent &event) {
    try {
        auto* newTimer = new Timer("Timer"+std::to_string(timers.size()+1),5);
        timers.push_back(newTimer);
        timerListBox->Append(newTimer->getName());
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for a new timer. Terminating." << std::endl;
        std::terminate();
    }

}

void MainFrame::OnRename(wxCommandEvent &event) {
    std::string newName = timerNameField->GetValue().ToStdString();
    auto it = std::find(timers.begin(), timers.end(), currentTimer);
    int timerListBoxIndex = static_cast<int>(std::distance(timers.begin(), it));
    timerListBox->SetString(timerListBoxIndex, newName);
    currentTimer->setName(newName);
}


void MainFrame::OnSelectCurrentTimer(wxCommandEvent &event) {
    int selection = timerListBox->GetSelection();
    currentTimer = timers[selection];
    updateSpinCtrlValues();
    update();
}

void MainFrame::updateCurrentTimerDuration(wxSpinEvent &event) {
    int total = secondsSpinCtrl->GetValue();
    total += minutesSpinCtrl->GetValue()*60;
    total += hoursSpinCtrl->GetValue()*3600;
    total += daysSpinCtrl->GetValue()*3600*24;
    total += weeksSpinCtrl->GetValue()*3600*24*7;
    total += yearsSpinCtrl->GetValue()*3600*24*365;
    currentTimer->setDuration(total);
    update();
}

void MainFrame::updateSpinCtrlValues() {
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

