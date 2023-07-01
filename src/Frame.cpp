#include "Frame.h"
#include <filesystem>
auto newIcon = std::filesystem::path{"../res/new.png"};
auto deleteIcon = std::filesystem::path{"../res/delete.png"};
auto editIcon = std::filesystem::path{"../res/edit.png"};

wxBEGIN_EVENT_TABLE(frame, wxFrame)
                EVT_MENU(wxID_EXIT, frame::onMenuFileQuit)
                EVT_MENU(MenuEditOptions, frame::onMenuEditOptions)
                EVT_BUTTON(StartButton, frame::onStart)
                EVT_BUTTON(StopButton, frame::onStop)
                EVT_BUTTON(wxID_NEW, frame::onNew)
                EVT_BUTTON(DeleteButton, frame::onDelete)
                EVT_BUTTON(RenameButton, frame::onRename)
                EVT_LISTBOX(ListBox, frame::onTimerSelection)
                EVT_SPINCTRL(SpinCtrlId, frame::onTimeParameterChange)
                EVT_CLOSE(frame::onCloseWindow)
wxEND_EVENT_TABLE()

frame::frame(const std::string &title, observer *controller) : wxFrame(nullptr, wxID_ANY, title),
                                                               mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))),
                                                               controller(controller) {
    allocateUiMemory();
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
        remainingTimeSizer = new wxBoxSizer(wxHORIZONTAL);
        parameters = new wxBoxSizer(wxHORIZONTAL);
        timerStartStop = new wxBoxSizer(wxHORIZONTAL);
        yearsHoursParameters = new wxBoxSizer(wxVERTICAL);
        weeksMinutesParameters = new wxBoxSizer(wxVERTICAL);
        daysSecondsParameters = new wxBoxSizer(wxVERTICAL);
        hourPar = new wxBoxSizer(wxVERTICAL);
        minutePar = new wxBoxSizer(wxVERTICAL);
        secondPar = new wxBoxSizer(wxVERTICAL);
        timerListStaticText = new wxStaticText(mainPanel, wxID_ANY, wxString("Saved Timers"));
        timerListBox = new wxListBox(mainPanel, ListBox, wxDefaultPosition, wxSize(200, 300), savedTimers);
        newBitmapButton = new wxBitmapButton(mainPanel, wxID_NEW, wxBitmapBundle(wxIcon(newIcon.make_preferred().string())));
        deleteBitmapButton = new wxBitmapButton(mainPanel, DeleteButton, wxBitmapBundle(wxIcon(deleteIcon.make_preferred().string())));
        renameBitmapButton = new wxBitmapButton(mainPanel, RenameButton, wxBitmapBundle(wxIcon(editIcon.make_preferred().string())));

        timerNameField = new wxStaticText(mainPanel, NameField, "");

        yearsLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Years"));
        weeksLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Weeks"));
        daysLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Days"));
        hoursLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Hours"));
        minutesLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Minutes"));
        secondsLabel = new wxStaticText(mainPanel, wxID_ANY, wxString("Seconds"));
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

        startButton = new wxButton(mainPanel, StartButton, wxString("Start"));
        stopButton = new wxButton(mainPanel, StopButton, wxString("Stop"));
        remainingTimeStaticText = new wxStaticText(mainPanel, wxID_ANY, wxString(""));
        startDateText = new wxStaticText(mainPanel, wxID_ANY, wxString(""));
        endDateText = new wxStaticText(mainPanel, wxID_ANY, wxString(""));
    } catch (const std::bad_alloc &e) {
        showMemoryError(true);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "mannaggia" << std::endl;
    }
}

void frame::setupUi() {
    if (menuBar != nullptr && fileMenu != nullptr && editMenu != nullptr) {
        menuBar->Append(fileMenu, _T("&File"));
        menuBar->Append(editMenu, _T("&Edit"));
        fileMenu->Append(wxID_EXIT, _T("&Quit"));
        editMenu->Append(MenuEditOptions, _T("&Options"));
    }

    if (columns != nullptr && leftColumn != nullptr && rightColumn != nullptr) {
        columns->Add(leftColumn, wxSizerFlags().Border(wxLEFT, 25));
        columns->Add(rightColumn, wxSizerFlags().Border(wxLEFT, 30));
    }

    if (timerListStaticText != nullptr && timerListBox != nullptr && timerManagementButtons != nullptr) {
        leftColumn->AddSpacer(10);
        leftColumn->Add(timerListStaticText, wxSizerFlags().Center());
        leftColumn->Add(timerListBox, wxSizerFlags().Border(wxUP, 5));
        leftColumn->Add(timerManagementButtons, wxSizerFlags().Center().Border(wxUP, 7));
    }

    if (newBitmapButton != nullptr && deleteBitmapButton != nullptr && renameBitmapButton != nullptr) {
        timerManagementButtons->Add(newBitmapButton, wxSizerFlags().Center());
        timerManagementButtons->Add(deleteBitmapButton, wxSizerFlags().Center().Border(wxLEFT, 7));
        timerManagementButtons->Add(renameBitmapButton, wxSizerFlags().Center().Border(wxLEFT, 7));
    }

    if (timerNameSizer != nullptr && remainingTimeSizer != nullptr && parameters != nullptr &&
        timerStartStop != nullptr) {
        rightColumn->AddSpacer(10);
        rightColumn->Add(timerNameSizer, wxSizerFlags().Center());
        rightColumn->Add(remainingTimeSizer, wxSizerFlags().Border(wxUP, 5).Center());
        rightColumn->Add(parameters, wxSizerFlags().Border(wxUP, 5).Center());
        rightColumn->Add(timerStartStop, wxSizerFlags().Border(wxUP, 15).Center());
    }

    if (yearsHoursParameters != nullptr && weeksMinutesParameters != nullptr && daysSecondsParameters != nullptr &&
        hourPar != nullptr && minutePar != nullptr && secondPar != nullptr) {
        parameters->Add(yearsHoursParameters, wxSizerFlags().Center());
        parameters->Add(weeksMinutesParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(daysSecondsParameters, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(hourPar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(minutePar, wxSizerFlags().Border(wxLEFT, 5).Center());
        parameters->Add(secondPar, wxSizerFlags().Border(wxLEFT, 5).Center());
    }

    if (timerNameField != nullptr) {
        timerNameSizer->Add(timerNameField, wxSizerFlags().Center());
        timerNameField->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    }

    if (remainingTimeStaticText != nullptr) {
        remainingTimeSizer->AddStretchSpacer();
        remainingTimeSizer->Add(remainingTimeStaticText, wxSizerFlags(wxALIGN_CENTER).Border(wxUP, 10));
        remainingTimeSizer->AddStretchSpacer();
        remainingTimeStaticText->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    }

    if (yearsLabel != nullptr && yearsSpinCtrl != nullptr && hoursLabel != nullptr && hoursSpinCtrl != nullptr) {
        yearsHoursParameters->Add(yearsLabel, wxSizerFlags().Center());
        yearsHoursParameters->Add(yearsSpinCtrl);
        yearsHoursParameters->Add(hoursLabel, wxSizerFlags().Border(wxUP, 5).Center());
        yearsHoursParameters->Add(hoursSpinCtrl);
    }

    if (weeksLabel != nullptr && weeksSpinCtrl != nullptr && minutesLabel != nullptr && minutesSpinCtrl != nullptr) {
        weeksMinutesParameters->Add(weeksLabel, wxSizerFlags().Center());
        weeksMinutesParameters->Add(weeksSpinCtrl);
        weeksMinutesParameters->Add(minutesLabel, wxSizerFlags().Border(wxUP, 5).Center());
        weeksMinutesParameters->Add(minutesSpinCtrl);
    }

    if (daysLabel != nullptr && daysSpinCtrl != nullptr && secondsLabel != nullptr && secondsSpinCtrl != nullptr) {
        daysSecondsParameters->Add(daysLabel, wxSizerFlags().Center());
        daysSecondsParameters->Add(daysSpinCtrl);
        daysSecondsParameters->Add(secondsLabel, wxSizerFlags().Border(wxUP, 5).Center());
        daysSecondsParameters->Add(secondsSpinCtrl);
    }

    if (startButton != nullptr && stopButton != nullptr) {
        timerStartStop->Add(startButton, wxSizerFlags(wxALIGN_CENTER));
        timerStartStop->Add(stopButton, wxSizerFlags(wxALIGN_CENTER).Border(wxLEFT, 10));
    }

    if (startDateText != nullptr && endDateText != nullptr) {
        rightColumn->Add(startDateText, wxSizerFlags().Border(wxUP, 15));
        rightColumn->Add(endDateText, wxSizerFlags().Border(wxUP, 10));
    }

    SetMenuBar(menuBar);
    mainPanel->SetSizerAndFit(columns);
}

void frame::showMemoryError(const bool &critical) {
    wxMessageDialog sd = wxMessageDialog(this,
                                         wxString("Failed to allocate memory for new elements. Exiting the program."),
                                         "Memory error occurred", wxOK);
    sd.ShowModal();
    if (critical)
        exit(0);
}

void frame::onCloseWindow(wxCloseEvent &WXUNUSED(event)) {
    controller->closeWindow();
}

void frame::onMenuFileQuit(wxCommandEvent &WXUNUSED(event)) {
    controller->closeWindow();
}

void frame::onMenuEditOptions(wxCommandEvent &WXUNUSED(event)) {
    auto *options = new optionsFrame("Options", controller);
    int width = 550, height = 350;
    options->SetClientSize(width, height);
    options->SetMinClientSize(wxSize(width, height));
    options->SetMaxClientSize(wxSize(width, height));
    options->Center();
    options->ShowModal();
    options->Destroy();
}

void frame::onTimerSelection(wxCommandEvent &WXUNUSED(event)) {
    controller->updateTimerView();
}

void frame::onNew(wxCommandEvent &WXUNUSED(event)) {
    controller->createNewTimer();
}

void frame::onDelete(wxCommandEvent &WXUNUSED(event)) {
    controller->deleteSelectedTimer();
}

void frame::onRename(wxCommandEvent &WXUNUSED(event)) {
    controller->renameSelectedTimer();
}

void frame::onStart(wxCommandEvent &WXUNUSED(event)) {
    controller->startSelectedTimer();
}

void frame::onStop(wxCommandEvent &WXUNUSED(event)) {
    controller->stopSelectedTimer();
}

void frame::onTimeParameterChange(wxSpinEvent &WXUNUSED(event)) {
    controller->updateSelectedTimerDuration();
}
