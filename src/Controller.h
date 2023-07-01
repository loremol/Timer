#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H


#include "Frame.h"
#include "Observer.h"

class controller : public observer {
public:
    controller();

    void createNewTimer() override;

    void deleteSelectedTimer() override;

    void renameSelectedTimer() override;

    void startSelectedTimer() override;

    void stopSelectedTimer() override;

    void eraseTimerThread(const std::thread::id &threadId) override;

    void closeWindow() override;

    void updateTimerView() override;

    void layoutView() override;

    void updateControls() override;

    void updateRemainingTime() override;

    void updateTimerDates() override;

    void updateSelectedTimerDuration() override;

    void changeDateFormat(const std::string &newFormat) override;

    void changeTimerFormat(const std::string &newFormat) override;

    [[nodiscard]] const std::string &getTimerFormat() const override;

    [[nodiscard]] const std::string &getDateFormat() const override;

private:
    void loadTimers();

    void saveTimers();

    void updateSpinCtrlValues();

    void updateNameField();

    static void waitForTimerStop(const std::shared_ptr<timer> &timer);

    std::vector<std::shared_ptr<timer>> timers{};
    std::map<int, std::thread> threads{};
    frame *view;
    std::shared_ptr<timer> selectedTimer = nullptr;
    std::string timerFormat{"%M:%S"};
    std::string dateFormat{"%H:%M:%S, %d/%m/%Y"};
};


#endif //TIMER_CONTROLLER_H
