#ifndef TIMER_OPTIONSFRAME_H
#define TIMER_OPTIONSFRAME_H

#include <string>
#include <wx/wx.h>

class optionsFrame : public wxDialog {
public:
    explicit optionsFrame(const std::string &title);

private:
    wxPanel *mainPanel{};

};


#endif //TIMER_OPTIONSFRAME_H
