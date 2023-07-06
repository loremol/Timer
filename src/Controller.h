#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H


#include "Frame.h"
#include "ControllerInterface.h"

class controller : public controllerInterface {
public:
    controller();

    ~controller() override { saveAndClose(); };

    controller(const controller &controller1) = delete;

    controller operator=(const controller &controller1) = delete;

    void createNewTimer() override;

    void deleteSelectedTimer() override;

    void renameSelectedTimer() override;

    void startSelectedTimer() override;

    void stopSelectedTimer() override;

    void eraseTimerThread(const std::string &threadId) override;

    void closeWindow() override;

    void updateTimerView() override;

    void layoutView() override;

    void updateControls() override;

    void updateRemainingTime() override;

    void updateTimerDates() override;

    void updateSelectedTimerDuration() override;

    void changeDateFormat(const std::string &newFormat) override;

    void changeTimerFormat(const std::string &newFormat) override;

    [[nodiscard]] wxFrame *getView() const override;

    [[nodiscard]] const std::vector<std::shared_ptr<timer>> &getTimers() const;

    [[nodiscard]] const std::shared_ptr<timer> &getSelectedTimer() const;

    [[nodiscard]] const std::map<int, std::thread> &getThreads() const;

    [[nodiscard]] const std::string &getTimerFormat() const override;

    [[nodiscard]] const std::string &getDateFormat() const override;

private:
    void saveAndClose();

    void loadTimers();

    void saveTimers() const;

    void loadFormats();

    void saveFormats() const;

    void updateSpinCtrlValues();

    void updateNameField();

    void updateSelectedTimer();

    [[nodiscard]] std::string stringifyThreadId(const std::thread::id &threadId) const;

    std::vector<std::shared_ptr<timer>> timers{};
    std::map<int, std::thread> threads{};
    frame *view;
    std::shared_ptr<timer> selectedTimer{nullptr};
    std::string timerFormat{"%M:%S"};
    std::string dateFormat{"%H:%M:%S, %d %B %Y"};

};


#endif //TIMER_CONTROLLER_H
