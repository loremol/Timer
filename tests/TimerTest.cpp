#include "../src/Timer.h"

#include <gtest/gtest.h>
#include <thread>

class timerTest : public ::testing::Test {
protected:
    std::unique_ptr<timer> t;
    std::string timerName;
    int duration{0};

    void SetUp() override {
        t = std::make_unique<timer>(timerName, 0, nullptr);
    };

    void TearDown() override {
        t.reset();
    }
};

TEST_F(timerTest, Constructor) {
    ASSERT_EQ(t->getName(), timerName);
    ASSERT_EQ(t->getDuration(), duration);
    ASSERT_EQ(t->isRunning(), false);
}

TEST_F(timerTest, StartStop) {
    t->setDuration(5);
    std::thread runningTimer{[this]() { t->start(); }};
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_EQ(t->isRunning(), true);
    t->requestStop();
    if (runningTimer.joinable())
        runningTimer.join();
    ASSERT_EQ(t->isRunning(), false);
}

TEST_F(timerTest, FormatRemainingTime) {
    t->setDuration(300);
    ASSERT_EQ(t->formatRemainingTime("%M:%S"), "5m:0s");
    t->setDuration(3600 * 25);
    ASSERT_EQ(t->formatRemainingTime("%d, %H:%M:%S"), "1d, 1h:0m:0s");
    std::thread runningTimer{[this]() { t->start(); }};
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(t->formatRemainingTime("%d, %H:%M:%S"), "1d, 0h:59m:59s");
    t->requestStop();
    if (runningTimer.joinable())
        runningTimer.join();
}