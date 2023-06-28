#ifndef TIMER_OBSERVER_H
#define TIMER_OBSERVER_H

#include <thread>

class observer {
public:
    virtual ~observer() = default;

    virtual void updateTimerView() = 0;

    virtual void closeWindow() = 0;

    virtual void createNewTimer() = 0;

    virtual void deleteSelectedTimer() = 0;

    virtual void renameSelectedTimer() = 0;

    virtual void startSelectedTimer() = 0;

    virtual void stopSelectedTimer() = 0;

    virtual void eraseTimerThread(const std::thread::id &threadId) = 0;

    virtual void updateSelectedTimerDuration() = 0;

    virtual void layoutView() = 0;

    virtual void updateControls() = 0;

    virtual void updateRemainingTime() = 0;

    virtual void updateTimerDates() = 0;
};

#endif //TIMER_OBSERVER_H
