#ifndef TIMER_APP_H
#define TIMER_APP_H

#include <wx/wx.h>

class App : public wxApp {
    bool OnInit() override;
};

DECLARE_APP(App)

#endif //TIMER_APP_H
