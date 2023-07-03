#include "App.h"

bool app::OnInit() {
    try {
        c = std::make_shared<controller>();
        return true;
    } catch (const std::bad_alloc &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}
