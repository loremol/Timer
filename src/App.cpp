#include "App.h"
#include "Frame.h"

bool App::OnInit() {
    auto *mainFrame = new frame("Timer");
    mainFrame->SetClientSize(635, 375);
    mainFrame->SetMinClientSize(wxSize(635,375));
    mainFrame->SetMaxClientSize(wxSize(635,375));
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

IMPLEMENT_APP(App)