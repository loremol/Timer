#include "App.h"
#include "Frame.h"

bool App::OnInit() {
    auto *mainFrame = new frame("Timer");
    mainFrame->SetClientSize(635, 410);
    mainFrame->SetMinClientSize(wxSize(635, 410));
    mainFrame->SetMaxClientSize(wxSize(635, 410));
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

IMPLEMENT_APP(App)