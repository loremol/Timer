#include "App.h"
#include "Frame.h"

bool app::OnInit() {
    try {
        auto *mainFrame = new frame("Timer");
        mainFrame->SetClientSize(635, 415);
        mainFrame->SetMinClientSize(wxSize(635, 415));
        mainFrame->SetMaxClientSize(wxSize(635, 415));
        mainFrame->Center();
        mainFrame->Show();
        return true;
    } catch (const std::bad_alloc &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

IMPLEMENT_APP(app)

