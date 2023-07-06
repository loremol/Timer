#include "OptionsFrame.h"
#include "FormatInfoDialog.h"
#include <filesystem>

auto infoIcon = std::filesystem::path{"../res/info.png"};

wxBEGIN_EVENT_TABLE(optionsFrame, wxDialog)
                EVT_TEXT(TimerFormat, optionsFrame::onTimerFormatChange)
                EVT_TEXT(DateFormat, optionsFrame::onDateFormatChange)
                EVT_BUTTON(DateFormatInfo, optionsFrame::onDateFormatInfo)
                EVT_BUTTON(TimerFormatInfo, optionsFrame::onTimerFormatInfo)
wxEND_EVENT_TABLE()

optionsFrame::optionsFrame(const std::string &title, controllerInterface *observer) : wxDialog(nullptr, wxID_ANY,
                                                                                               title),
                                                                                      controller(observer) {
    allocateUiMemory();
    setupUi();
}

void optionsFrame::allocateUiMemory() {
    try {
        mainPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0));
        column = new wxBoxSizer(wxVERTICAL);
        timerFormatRow = new wxBoxSizer(wxHORIZONTAL);
        timerFormatPrompt = new wxStaticText(mainPanel, wxID_ANY, "Timer format");
        timerFormat = new wxTextCtrl(mainPanel, TimerFormat, wxEmptyString, wxDefaultPosition, wxSize(150, -1));
        dateFormatRow = new wxBoxSizer(wxHORIZONTAL);
        dateFormatPrompt = new wxStaticText(mainPanel, wxID_ANY, "Date format");
        dateFormat = new wxTextCtrl(mainPanel, DateFormat, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
        auto bit = wxBitmap(infoIcon.make_preferred().string());
        dateFormatInfo = new wxBitmapButton(mainPanel, DateFormatInfo, bit, wxDefaultPosition, wxDefaultSize);
        timerFormatInfo = new wxBitmapButton(mainPanel, TimerFormatInfo, bit, wxDefaultPosition, wxDefaultSize);
    } catch (const std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
        this->Destroy();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void optionsFrame::setupUi() {
    column->Add(timerFormatRow);
    column->Add(dateFormatRow);
    timerFormatRow->Add(timerFormatPrompt, wxSizerFlags().Align(wxLeft).Border(wxLEFT | wxTOP, 10));
    timerFormatRow->AddStretchSpacer();
    timerFormatRow->Add(timerFormat, wxSizerFlags().Align(wxRight).Border(wxLEFT | wxRIGHT | wxTOP, 10));
    timerFormatRow->Add(timerFormatInfo, wxSizerFlags().Align(wxRight).Border(wxTOP | wxRIGHT, 10));

    dateFormatRow->Add(dateFormatPrompt, wxSizerFlags().Align(wxLeft).Border(wxLEFT | wxTOP, 10));
    dateFormatRow->AddStretchSpacer();
    dateFormatRow->Add(dateFormat, wxSizerFlags().Align(wxRight).Border(wxLEFT | wxRIGHT | wxTOP, 10));
    dateFormatRow->Add(dateFormatInfo, wxSizerFlags().Align(wxRight).Border(wxTOP | wxRIGHT, 10));
    timerFormat->SetValue(controller->getTimerFormat());
    dateFormat->SetValue(controller->getDateFormat());
    mainPanel->SetSizerAndFit(column);
}

void optionsFrame::onTimerFormatChange(wxCommandEvent &WXUNUSED(event)) {
    if (timerFormat != nullptr) {
        controller->changeTimerFormat(timerFormat->GetValue().ToStdString());
        controller->updateRemainingTime();
        controller->layoutView();
    }
}

void optionsFrame::onDateFormatChange(wxCommandEvent &WXUNUSED(event)) {
    if (dateFormat != nullptr) {
        controller->changeDateFormat(dateFormat->GetValue().ToStdString());
        controller->updateTimerDates();
        controller->layoutView();
    }
}

void optionsFrame::onDateFormatInfo(wxCommandEvent &WXUNUSED(event)) {
    auto *formatInfo = new formatInfoDialog("Formats info",
                                            "%d = dd\n%e = d\n%f = m\n%m = mm\n%b = Mth\n%B = Month\n%Y = yyyy\n%y = yy\n%H = hh\n%I = hh (AM/PM)\n%p = AM/PM\n%M = mm\n%S = ss");
    int width = 175, height = 300;
    formatInfo->SetClientSize(width, height);
    formatInfo->SetMinClientSize(wxSize(width, height));
    formatInfo->SetMaxClientSize(wxSize(width, height));
    formatInfo->Center();
    formatInfo->ShowModal();
    formatInfo->Destroy();

}

void optionsFrame::onTimerFormatInfo(wxCommandEvent &WXUNUSED(event)) {
    auto *formatInfo = new formatInfoDialog("Formats info",
                                            "%y = years\n%m = months\n%w = weeks\n%d = days\n%H = hours\n%M = minutes\n%S = seconds");
    int width = 175, height = 300;
    formatInfo->SetClientSize(width, height);
    formatInfo->SetMinClientSize(wxSize(width, height));
    formatInfo->SetMaxClientSize(wxSize(width, height));
    formatInfo->Center();
    formatInfo->ShowModal();
    formatInfo->Destroy();
}
