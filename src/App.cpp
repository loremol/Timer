#include "App.h"

bool app::OnInit() {
    try {
        c = new controller();
        return true;
    } catch (const std::bad_alloc &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

int app::OnExit() {
    return 0;
}
