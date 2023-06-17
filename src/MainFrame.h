#ifndef TIMER_MAINFRAME_H
#define TIMER_MAINFRAME_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <thread>
#include "Timer.h"
#include "App.h"

enum eventID {
    LISTBOX,
    STARTBUTTON,
    STOPBUTTON,
    NEWBUTTON,
    RENAMEBUTTON,
    NAMEFIELD,
    YEARSC,
    WEEKSC,
    DAYSC,
    HOURSC,
    MINUTESC,
    SECONDSC
};

class MainFrame : public wxFrame {
public:
    explicit MainFrame(const std::string &title);

private:
    void setupUI();

    void loadTimers();

    void initUI();

    void update();

    const std::function<void()> updateView = [this]() {
        wxGetApp().CallAfter([=]() { this->update(); });
    };

    void OnStart(wxCommandEvent &event);

    void OnStop(wxCommandEvent &event);

    void OnNew(wxCommandEvent& event);

    void OnRename(wxCommandEvent& event);

    void OnSelectCurrentTimer(wxCommandEvent& event);

    void updateSpinCtrlValues();

    void updateCurrentTimerDuration(wxSpinEvent &event);

    std::vector<Timer *> timers;
    std::thread thread;
    Timer *currentTimer{};
    wxPanel *mainPanel;
    wxStaticText *timerListStaticText{};
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

    void setupEventHandling();
};

#endif //TIMER_MAINFRAME_H
