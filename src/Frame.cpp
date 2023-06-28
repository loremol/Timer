#include "Frame.h"

wxBEGIN_EVENT_TABLE(frame, wxFrame)
                EVT_MENU(wxID_EXIT, frame::onMenuFileQuit)
                EVT_MENU(MenuEditOptions, frame::onMenuEditOptions)
                EVT_BUTTON(StartButton, frame::onStart)
                EVT_BUTTON(StopButton, frame::onStop)
                EVT_BUTTON(NewButton, frame::onNew)
                EVT_BUTTON(DeleteButton, frame::onDelete)
                EVT_BUTTON(RenameButton, frame::onRename)
                EVT_LISTBOX(ListBox, frame::onNewSelection)
                EVT_SPINCTRL(SpinCtrlId, frame::onTimeParameterChange)
                EVT_CLOSE(frame::onCloseWindow)
wxEND_EVENT_TABLE()

frame::frame(const std::string &title, observer &controller) : wxFrame(nullptr, wxID_ANY, title),
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
        endDateText = new wxStaticText(mainPanel, wxID_ANY, "");
    } catch (const std::bad_alloc &e) {
        showMemoryError(true);
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
    rightColumn->Add(remainingTimeSizer, wxSizerFlags().Border(wxUP, 5).Center());
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

    remainingTimeSizer->AddStretchSpacer();
    remainingTimeSizer->Add(remainingTimeStaticText, wxSizerFlags(wxALIGN_CENTER).Border(wxUP, 10));
    remainingTimeSizer->AddStretchSpacer();
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
    rightColumn->Add(endDateText, wxSizerFlags().Border(wxUP, 10));

    SetMenuBar(menuBar);
    mainPanel->SetSizerAndFit(columns);
}

void frame::showMemoryError(const bool &critical) {
    wxMessageDialog sd = wxMessageDialog(this, "Failed to allocate memory for the UI elements. Exiting the program.",
                                         "Memory error occurred", wxOK);
    sd.ShowModal();
    if (critical)
        exit(0);
}

void frame::onCloseWindow(wxCloseEvent &WXUNUSED(event)) {
    controller.closeWindow();
}

void frame::onMenuFileQuit(wxCommandEvent &WXUNUSED(event)) {
    controller.closeWindow();
}

void frame::onMenuEditOptions(wxCommandEvent &WXUNUSED(event)) {
    auto *options = new optionsFrame("Options");
    int width = 550, height = 350;
    options->SetClientSize(width, height);
    options->SetMinClientSize(wxSize(width, height));
    options->SetMaxClientSize(wxSize(width, height));
    options->Center();
    options->ShowModal();
    delete options;
}

void frame::onNewSelection(wxCommandEvent &WXUNUSED(event)) {
    controller.updateTimerView();
}

void frame::onNew(wxCommandEvent &WXUNUSED(event)) {
    controller.createNewTimer();
}

void frame::onDelete(wxCommandEvent &WXUNUSED(event)) {
    controller.deleteSelectedTimer();
}

void frame::onRename(wxCommandEvent &WXUNUSED(event)) {
    controller.renameSelectedTimer();
}

void frame::onStart(wxCommandEvent &WXUNUSED(event)) {
    controller.startSelectedTimer();
}

void frame::onStop(wxCommandEvent &WXUNUSED(event)) {
    controller.stopSelectedTimer();
}

void frame::onTimeParameterChange(wxSpinEvent &WXUNUSED(event)) {
    controller.updateSelectedTimerDuration();
}
