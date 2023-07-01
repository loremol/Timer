#ifndef TIMER_FORMATINFODIALOG_H
#define TIMER_FORMATINFODIALOG_H

#include <wx/wx.h>

class formatInfoDialog : public wxDialog{
public:
    explicit formatInfoDialog(const std::string &title, const std::string &info);

private:
    wxPanel *mainPanel{};
    wxSizer *column{};
    wxStaticText *formatList{};
};


#endif //TIMER_FORMATINFODIALOG_H
