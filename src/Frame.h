#ifndef TIMER_FRAME_H
#define TIMER_FRAME_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <thread>
#include "Timer.h"
#include "App.h"

enum eventId {
    ListBox,
    StartButton,
    StopButton,
    NewButton,
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

    void initUi();

    void update();

    const std::function<void()> updateView = [this]() {
        wxGetApp().CallAfter([this]() { this->update(); });
    };

    void onStart(wxCommandEvent &event);

    void onStop(wxCommandEvent &event);

    void onNew(wxCommandEvent& event);

    void onRename(wxCommandEvent& event);

    void onSelectCurrentTimer(wxCommandEvent& event);

    void updateSpinCtrlValues();

    void updateCurrentTimerDuration(wxSpinEvent &event);

    std::vector<Timer *> timers;
    std::thread thread;
    Timer *currentTimer{};
    wxPanel *mainPanel;
    wxStaticText *timerListStaticText{};
    wxStaticText *startDateText{};
    wxStaticText *stopDateText{};
    wxListBox *timerListBox{};
    wxArrayString savedTimers;
    wxStaticText *remainingTimeStaticText{};
    wxButton *startButton{};
    wxButton *stopButton{};
    wxButton *newButton{};
    wxButton *renameButton{};
    wxSpinCtrl *yearsSpinCtrl{};
    wxSpinCtrl *weeksSpinCtrl{};
    wxSpinCtrl *daysSpinCtrl{};
    wxSpinCtrl *hoursSpinCtrl{};
    wxSpinCtrl *minutesSpinCtrl{};
    wxSpinCtrl *secondsSpinCtrl{};
    wxTextCtrl* timerNameField{};

    wxDECLARE_EVENT_TABLE();
};

#endif //TIMER_FRAME_H
