#include <fstream>
#include <sstream>
#include "MainFrame.h"
#include "App.h"

MainFrame::MainFrame(const std::string &title) : wxFrame(nullptr, wxID_ANY, title),
                                                 mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {
    initUI();
    loadTimers();
    setupUI();
}

void MainFrame::initUI() {
    try {
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, "Saved Timers");
        startButton = new wxButton(mainPanel, STARTBUTTON, "Start");
        stopButton = new wxButton(mainPanel, STOPBUTTON, "Stop");
        timerListBox = new wxListBox(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 300), savedTimers);
    } catch (const std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
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

        name->Add(new wxButton(mainPanel, wxID_ANY, "New"), wxSizerFlags(wxALIGN_CENTER));
        name->Add(new wxTextCtrl(mainPanel, wxID_ANY, currentTimer->getName()),
                  wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));
        name->Add(new wxButton(mainPanel, wxID_ANY, "Update"), wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

        year_par->Add(new wxStaticText(mainPanel, wxID_ANY, "years"), wxSizerFlags().Center());
        month_par->Add(new wxStaticText(mainPanel, wxID_ANY, "months"), wxSizerFlags().Center());
        day_par->Add(new wxStaticText(mainPanel, wxID_ANY, "days"), wxSizerFlags().Center());

        year_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));
        month_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));
        day_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));
        year_par->Add(new wxStaticText(mainPanel, wxID_ANY, "hours"), wxSizerFlags().Border(wxUP, 5).Center());
        year_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));
        month_par->Add(new wxStaticText(mainPanel, wxID_ANY, "minutes"), wxSizerFlags().Border(wxUP, 5).Center());
        month_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));
        day_par->Add(new wxStaticText(mainPanel, wxID_ANY, "seconds"), wxSizerFlags().Border(wxUP, 5).Center());
        day_par->Add(new wxSpinCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1)));

        controls->Add(startButton, wxSizerFlags(wxALIGN_CENTER));
        controls->Add(stopButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));

        remainingTimeStaticText = new wxStaticText(mainPanel, wxID_ANY, std::to_string(currentTimer->getRemaining()));
        remainingTimeStaticText->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        rightColumn->Add(remainingTimeStaticText, wxSizerFlags().Border(wxUP, 5).Center());
        mainPanel->SetSizerAndFit(columns);
    } catch (const std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
        std::terminate();
    }
    setupEventHandling();
    update();
}

void MainFrame::setupEventHandling() {
    startButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnStart), nullptr, this);
    stopButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnStop), nullptr, this);
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
    if (currentTimer->getState() == RUNNING) {
        startButton->Enable(false);
        stopButton->Enable(true);
    } else {
        startButton->Enable(true);
        stopButton->Enable(false);
    }
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(STARTBUTTON, MainFrame::OnStart)
                EVT_BUTTON(STOPBUTTON, MainFrame::OnStop)
wxEND_EVENT_TABLE()

