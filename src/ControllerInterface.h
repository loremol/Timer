#ifndef TIMER_CONTROLLERINTERFACE_H
#define TIMER_CONTROLLERINTERFACE_H


class controllerInterface {
public:
    virtual ~controllerInterface() = default;

    virtual void updateTimerView() = 0;

    virtual void closeWindow() = 0;

    virtual void createNewTimer() = 0;

    virtual void deleteSelectedTimer() = 0;

    virtual void renameSelectedTimer() = 0;

    virtual void startSelectedTimer() = 0;

    virtual void stopSelectedTimer() = 0;

    virtual void eraseTimerThread(const std::string &threadId) = 0;

    virtual void updateSelectedTimerDuration() = 0;

    virtual void layoutView() = 0;

    virtual void updateControls() = 0;

    virtual void updateRemainingTime() = 0;

    virtual void updateTimerDates() = 0;

    virtual void changeTimerFormat(const std::string &newFormat) = 0;

    virtual void changeDateFormat(const std::string &format) = 0;

    [[nodiscard]] virtual wxFrame *getView() const = 0;

    [[nodiscard]] virtual const std::string &getTimerFormat() const = 0;

    [[nodiscard]] virtual const std::string &getDateFormat() const = 0;

};

#endif //TIMER_CONTROLLERINTERFACE_H
