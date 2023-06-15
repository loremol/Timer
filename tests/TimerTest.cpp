#include "../src/Timer.h"

#include <gtest/gtest.h>

TEST(Timer, Constructor) {
    int duration = 5;
    std::string name = "Timer";
    Timer t(name, duration);
    ASSERT_EQ(t.getName(), name);
    ASSERT_EQ(t.getDuration(), duration);
    ASSERT_EQ(t.getRemaining(), duration);
    ASSERT_EQ(t.isRunning(), 0);
}
