#include "App.h"
#include "Frame.h"

bool app::OnInit() {
    try {
        auto *mainFrame = new frame("Timer");
        int width = 650, height = 400;
        mainFrame->SetClientSize(width, height);
        mainFrame->SetMinClientSize(wxSize(width, height));
        mainFrame->SetMaxClientSize(wxSize(width, height));
        mainFrame->Center();
        mainFrame->Show();
        return true;
    } catch (const std::bad_alloc &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

IMPLEMENT_APP(app)

