#include "App.h"

wxIMPLEMENT_APP_NO_MAIN(app);

int main(int argc, char **argv) {
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    return 0;
}