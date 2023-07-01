#ifndef TIMER_APP_H
#define TIMER_APP_H

#include <wx/wx.h>
#include "Controller.h"

class app : public wxApp {
public:
    ~app() override = default;

    bool OnInit() override;

    int OnExit() override;

    controller *c{};
};

wxDECLARE_APP(app);

#endif //TIMER_APP_H
