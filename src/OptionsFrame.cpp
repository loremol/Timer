#include "OptionsFrame.h"

optionsFrame::optionsFrame(const std::string &title) : wxDialog(nullptr, wxID_ANY, title),
                                                       mainPanel(new wxPanel(this, wxID_ANY, wxPoint(0, 0))) {

}
