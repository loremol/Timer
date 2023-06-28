#include "App.h"
#include "Controller.h"

bool app::OnInit() {
    try {
        auto *c = new controller();
        return true;
    } catch (const std::bad_alloc &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

IMPLEMENT_APP(app)
