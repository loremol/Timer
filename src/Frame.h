#ifndef TIMER_FRAME_H
#define TIMER_FRAME_H

#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <sstream>
#include <filesystem>
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

    const std::vector<wxStaticText *> & parameterTexts() {
        return parameterLabels;
    }

    const std::vector<wxSpinCtrl *> & timeControls() {
        return parameterControls;
    }

    [[nodiscard]] wxPanel *panel() const;

    [[nodiscard]] wxListBox *timerList() const;

    [[nodiscard]] wxStaticText *remainingTime() const;

    [[nodiscard]] wxStaticText *name() const;

    [[nodiscard]] wxStaticText *startDate() const;

    [[nodiscard]] wxStaticText *endDate() const;

    [[nodiscard]] wxBitmapButton *getDeleteBitmapButton() const;

    [[nodiscard]] wxBitmapButton *getRenameBitmapButton() const;

    [[nodiscard]] wxButton *getStartButton() const;

    [[nodiscard]] wxButton *getStopButton() const;


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

    void onTimerStop(wxCommandEvent &event);

    observer *controller;
    wxPanel *mainPanel{};
    wxMenuBar *menuBar{};
    wxMenu *fileMenu{}, *editMenu{};
    wxBoxSizer *columns{}, *leftColumn{}, *rightColumn{}, *timerManagementButtons{}, *timerNameSizer{}, *remainingTimeSizer{}, *parameters{}, *timerStartStop{};
    wxBoxSizer *yearsHoursParameters{}, *weeksMinutesParameters{}, *daysSecondsParameters{}, *hourPar{}, *minutePar{}, *secondPar{};
    wxListBox *timerListBox{};
    wxBitmapButton *newBitmapButton{}, *deleteBitmapButton{}, *renameBitmapButton{};
    wxButton *startButton{}, *stopButton{};
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
