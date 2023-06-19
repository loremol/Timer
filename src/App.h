#ifndef TIMER_APP_H
#define TIMER_APP_H

#include <wx/wx.h>

class app : public wxApp {
    bool OnInit() override;
};

DECLARE_APP(app)

#endif //TIMER_APP_H
