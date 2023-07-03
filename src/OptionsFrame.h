#ifndef TIMER_OPTIONSFRAME_H
#define TIMER_OPTIONSFRAME_H

#include <string>
#include <wx/wx.h>
#include "Observer.h"

enum optionsEventId {
    TimerFormat = wxID_HIGHEST + 1000,
    DateFormat,
    DateFormatInfo,
    TimerFormatInfo
};

class optionsFrame : public wxDialog {
public:
    explicit optionsFrame(const std::string &title, observer *observer);

private:
    void allocateUiMemory();

    void setupUi();

    void onTimerFormatChange(wxCommandEvent &event);

    void onDateFormatChange(wxCommandEvent &event);

    void onDateFormatInfo(wxCommandEvent &event);

    void onTimerFormatInfo(wxCommandEvent &event);

    observer *controller;
    wxPanel *mainPanel{};
    wxBoxSizer *column{}, *timerFormatRow{}, *dateFormatRow{};
    wxStaticText *timerFormatPrompt{}, *dateFormatPrompt{};
    wxTextCtrl *timerFormat{}, *dateFormat{};
    wxBitmapButton *dateFormatInfo{}, *timerFormatInfo{};

wxDECLARE_EVENT_TABLE();
};


#endif //TIMER_OPTIONSFRAME_H
