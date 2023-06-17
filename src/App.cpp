#include "App.h"
#include "MainFrame.h"

bool App::OnInit() {
    auto *mainFrame = new MainFrame("Timer");
    mainFrame->SetClientSize(635, 375);
    mainFrame->SetMinClientSize(wxSize(635,375));
    mainFrame->SetMaxClientSize(wxSize(635,375));
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

IMPLEMENT_APP(App)