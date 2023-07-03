#ifndef TIMER_APP_H
#define TIMER_APP_H

#include <wx/wx.h>
#include "Controller.h"

class app : public wxApp {
public:
    bool OnInit() override;

    std::shared_ptr<controller> c{};
};

wxDECLARE_APP(app);

#endif //TIMER_APP_H
