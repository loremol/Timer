#ifndef TIMER_FRAME_H
#define TIMER_FRAME_H

#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <list>
#include "Timer.h"
#include "Observer.h"

enum eventId {
    MenuEditOptions = wxID_HIGHEST + 1,
    ListBox,
    StartButton,
    StopButton,
    DeleteButton,
    RenameButton,
    NameField,
    SpinCtrlId,
};

class frame : public wxFrame {
public:
    frame(const std::string &title, observer *controller);

    void showMemoryError();

    std::vector<wxStaticText *> &parameterTexts() {
        return parameterLabels;
    }

    std::vector<wxSpinCtrl *> &timeControls() {
        return parameterControls;
    }

    wxListBox &timerList() {
        return *timerListBox;
    }

    wxStaticText &remainingTime() {
        return *remainingTimeStaticText;
    }

    wxStaticText &name() {
        return *timerNameField;
    }

    wxStaticText &startDate() {
        return *startDateText;
    }

    wxStaticText &endDate() {
        return *endDateText;
    }

    [[nodiscard]] wxPanel *panel() const {
        return mainPanel;
    }

    wxPanel *mainPanel{};
    wxBitmapButton *newBitmapButton{}, *deleteBitmapButton{}, *renameBitmapButton{};
    wxButton *startButton{}, *stopButton{};

private:
    void allocateUiMemory();

    void setupUi();

    void onMenuFileQuit(wxCommandEvent &event);

    void onCloseWindow(wxCloseEvent &event);

    void onMenuEditOptions(wxCommandEvent &event);

    void onStart(wxCommandEvent &event);

    void onStop(wxCommandEvent &event);

    void onNew(wxCommandEvent &event);

    void onDelete(wxCommandEvent &event);

    void onRename(wxCommandEvent &event);

    void onTimeParameterChange(wxSpinEvent &event);

    void onTimerSelection(wxCommandEvent &event);

    void onTimerStart(wxCommandEvent &event);

    void onTimerTick(wxCommandEvent &event);

    observer *controller;
    wxMenuBar *menuBar{};
    wxMenu *fileMenu{}, *editMenu{};
    wxBoxSizer *columns{}, *leftColumn{}, *rightColumn{}, *timerManagementButtons{}, *timerNameSizer{}, *remainingTimeSizer{}, *parameters{}, *timerStartStop{};
    wxBoxSizer *yearsHoursParameters{}, *weeksMinutesParameters{}, *daysSecondsParameters{}, *hourPar{}, *minutePar{}, *secondPar{};
    wxListBox *timerListBox{};
    wxStaticText *timerListStaticText{};
    wxStaticText *yearsLabel{}, *weeksLabel{}, *daysLabel{}, *hoursLabel{}, *minutesLabel{}, *secondsLabel{};
    wxStaticText *startDateText{}, *endDateText{};
    std::vector<wxStaticText *> parameterLabels{};
    std::vector<wxSpinCtrl *> parameterControls{};
    wxArrayString savedTimers;
    wxStaticText *remainingTimeStaticText{};
    wxSpinCtrl *yearsSpinCtrl{}, *weeksSpinCtrl{}, *daysSpinCtrl{}, *hoursSpinCtrl{}, *minutesSpinCtrl{}, *secondsSpinCtrl{};
    wxStaticText *timerNameField{};

wxDECLARE_EVENT_TABLE();
};

#endif //TIMER_FRAME_H
