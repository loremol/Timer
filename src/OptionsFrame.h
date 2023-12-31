#ifndef TIMER_OPTIONSFRAME_H
#define TIMER_OPTIONSFRAME_H

#include <string>
#include <wx/wx.h>
#include "ControllerInterface.h"

enum optionsEventId {
    TimerFormat = wxID_HIGHEST + 1000,
    DateFormat,
    DateFormatInfo,
    TimerFormatInfo
};

class optionsFrame : public wxDialog {
public:
    optionsFrame(const std::string &title, controllerInterface *observer);

private:
    void allocateUiMemory();

    void setupUi();

    void onTimerFormatChange(wxCommandEvent &event);

    void onDateFormatChange(wxCommandEvent &event);

    void onDateFormatInfo(wxCommandEvent &event);

    void onTimerFormatInfo(wxCommandEvent &event);

    static void showFormatInfoDialog(const std::string &information);

    controllerInterface *controller;
    wxPanel *mainPanel{};
    wxBoxSizer *column{}, *timerFormatRow{}, *dateFormatRow{};
    wxStaticText *timerFormatPrompt{}, *dateFormatPrompt{};
    wxTextCtrl *timerFormat{}, *dateFormat{};
    wxBitmapButton *dateFormatInfo{}, *timerFormatInfo{};

wxDECLARE_EVENT_TABLE();
};


#endif //TIMER_OPTIONSFRAME_H
