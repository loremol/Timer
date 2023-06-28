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

private:
    void loadTimers();

    void updateSpinCtrlValues();

    void updateNameField();

    static void waitForTimerStop(const std::shared_ptr<timer> &timer);

    std::vector<std::shared_ptr<timer>> timers{};
    std::map<int, std::thread> threads{};
    frame *view;
    std::shared_ptr<timer> selectedTimer = nullptr;
};


#endif //TIMER_CONTROLLER_H
