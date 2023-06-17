#include "App.h"
#include "MainFrame.h"

bool App::OnInit() {
    auto *mainFrame = new MainFrame("Timer");
    mainFrame->SetClientSize(700, 500);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

IMPLEMENT_APP(App)