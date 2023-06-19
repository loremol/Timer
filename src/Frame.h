#ifndef TIMER_FRAME_H
#define TIMER_FRAME_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <thread>
#include <map>
#include "Timer.h"
#include "App.h"

enum eventId {
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

    void onStart(wxCommandEvent &event);

    void onStop(wxCommandEvent &event);

    void onNew(wxCommandEvent &event);

    void onDelete(wxCommandEvent &event);

    void onRename(wxCommandEvent &event);

    void onNewTimerSelection(wxCommandEvent &event);

    void updateSpinCtrlValues();

    void updateCurrentTimerDuration(wxSpinEvent &event);

    void showMemoryError(const bool &critical);

    std::vector<std::shared_ptr<timer>> timers;
    std::map<int, std::thread> threads;
    std::shared_ptr<timer> currentTimer = nullptr;
    wxPanel *mainPanel;
    wxBoxSizer *columns{}, *leftColumn{}, *rightColumn{}, *name{}, *remainingTime{}, *parameters{}, *controls{}, *yearsHoursParameters{}, *weeksMinutesParameters{}, *daysSecondsParameters{}, *hourPar{}, *minutePar{}, *secondPar{};
    wxStaticText *timerListStaticText{};
    wxStaticText *yearsLabel{}, *weeksLabel{}, *daysLabel{}, *hoursLabel{}, *minutesLabel{}, *secondsLabel{};
    std::vector<wxStaticText *> parameterLabels{};
    std::vector<wxSpinCtrl *> parameterControls{};
    wxStaticText *startDateText{};
    wxStaticText *stopDateText{};
    wxListBox *timerListBox{};
    wxArrayString savedTimers;
    wxStaticText *remainingTimeStaticText{};
    wxButton *startButton{};
    wxButton *stopButton{};
    wxButton *newButton{};
    wxButton *deleteButton{};
    wxButton *renameButton{};
    wxSpinCtrl *yearsSpinCtrl{};
    wxSpinCtrl *weeksSpinCtrl{};
    wxSpinCtrl *daysSpinCtrl{};
    wxSpinCtrl *hoursSpinCtrl{};
    wxSpinCtrl *minutesSpinCtrl{};
    wxSpinCtrl *secondsSpinCtrl{};
    wxTextCtrl *timerNameField{};

wxDECLARE_EVENT_TABLE();
};

#endif //TIMER_FRAME_H
