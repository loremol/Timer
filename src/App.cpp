#include "App.h"
#include "MainFrame.h"

bool App::OnInit() {
    auto* mainFrame = new MainFrame("Timer");
    mainFrame->SetClientSize(800,600);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}
