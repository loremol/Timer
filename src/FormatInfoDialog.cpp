#include "FormatInfoDialog.h"

formatInfoDialog::formatInfoDialog(const std::string &title, const std::string &info) : wxDialog(nullptr, wxID_ANY,
                                                                                                 title) {
    try {
        mainPanel = new wxPanel(this);
        column = new wxBoxSizer(wxVERTICAL);
        formatList = new wxStaticText(mainPanel, wxID_ANY, wxString(info));
    } catch (const std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
    }

    column->Add(formatList, wxSizerFlags().Border(wxLEFT | wxTOP, 10));
    mainPanel->SetSizerAndFit(column);
}
