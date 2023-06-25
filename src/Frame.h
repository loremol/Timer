#ifndef TIMER_FRAME_H
#define TIMER_FRAME_H

#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <sstream>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Timer.h"
#include "App.h"

enum eventId {
    MenuEditOptions,
    ListBox,
    StartButton,
    StopButton,
    NewButton,
    DeleteButton,
    RenameButton,
    NameField,
    SpinCtrlId,
};

class frame : public wxFrame {
public:
    explicit frame(const std::string &title);

private:
    void setupUi();

    void loadTimers();

    void allocateUiMemory();

    void updateTimerDates();

    void updateControls();

    void updateRemainingTime();

    void updateNameField();

    const std::function<void()> updateView = [this]() {
        wxGetApp().CallAfter([this]() {
            this->updateRemainingTime();
            this->updateControls();
            this->updateTimerDates();
            mainPanel->Layout();
        });
    };

    void onMenuFileQuit(wxCommandEvent &event);

    void onMenuEditOptions(wxCommandEvent &event);

    void onStart(wxCommandEvent &event);

    void onStop(wxCommandEvent &event);

    void onNew(wxCommandEvent &event);

    void onDelete(wxCommandEvent &event);

    void onRename(wxCommandEvent &event);

    void onNewTimerSelection(wxCommandEvent &event);

    void updateSpinCtrlValues();

    void updateCurrentTimerDuration(wxSpinEvent &event);

    void onCloseWindow(wxCloseEvent &event);

    void showMemoryError(const bool &critical);

    std::vector<std::shared_ptr<timer>> timers{};
    std::map<int, std::thread> threads{};
    std::shared_ptr<timer> currentTimer = nullptr;
    wxPanel *mainPanel{};
    wxMenuBar *menuBar{};
    wxMenu *fileMenu{}, *editMenu{};
    wxBoxSizer *columns{}, *leftColumn{}, *rightColumn{}, *timerManagementButtons{}, *timerNameSizer{}, *remainingTime{}, *parameters{}, *timerStartStop{};
    wxBoxSizer *yearsHoursParameters{}, *weeksMinutesParameters{}, *daysSecondsParameters{}, *hourPar{}, *minutePar{}, *secondPar{};
    wxBitmapButton *newBitmapButton{}, *deleteBitmapButton{}, *renameBitmapButton{};
    wxStaticText *timerListStaticText{};
    wxStaticText *yearsLabel{}, *weeksLabel{}, *daysLabel{}, *hoursLabel{}, *minutesLabel{}, *secondsLabel{};
    wxStaticText *startDateText{}, *stopDateText{};
    std::vector<wxStaticText *> parameterLabels{};
    std::vector<wxSpinCtrl *> parameterControls{};
    wxListBox *timerListBox{};
    wxArrayString savedTimers;
    wxStaticText *remainingTimeStaticText{};
    wxButton *startButton{}, *stopButton{};
    wxSpinCtrl *yearsSpinCtrl{}, *weeksSpinCtrl{}, *daysSpinCtrl{}, *hoursSpinCtrl{}, *minutesSpinCtrl{}, *secondsSpinCtrl{};
    wxStaticText *timerNameField{};

wxDECLARE_EVENT_TABLE();

    void startCurrentTimer();

    void updateSelection();
};

#endif //TIMER_FRAME_H
