#ifndef TIMER_APP_H
#define TIMER_APP_H

#include <wx/wx.h>

class app : public wxApp {
public:
    bool OnInit() override;
};

wxDECLARE_APP(app);

#endif //TIMER_APP_H
