#include "FormatInfoDialog.h"

formatInfoDialog::formatInfoDialog(const std::string &title, const std::string &info) : wxDialog(nullptr, wxID_ANY,
                                                                                                 title) {
    mainPanel = new wxPanel(this);
    column = new wxBoxSizer(wxVERTICAL);
    formatList = new wxStaticText(mainPanel, wxID_ANY, wxString(info));
    column->Add(formatList, wxSizerFlags().Border(wxLEFT | wxTOP, 10));
    mainPanel->SetSizerAndFit(column);
}
