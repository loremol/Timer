#include <gtest/gtest.h>
#include <wx/wx.h>
#include "../src/App.h"

wxIMPLEMENT_APP_NO_MAIN(app);

class controllerTest : public testing::Test {
protected:
    app *myApp{};
    std::shared_ptr<controller> con;
    size_t timers{}, threads{};

    void SetUp() override {
        remove("timers.txt");
        char name[] = "wxUnitTest";
        int argc = 1;
        char *argv[1] = {name};
        myApp = new app();
        wxApp::SetInstance(myApp);
        wxEntryStart(argc, argv);
        myApp->OnInit();
        con = myApp->c;
    }

    void TearDown() override {
        con.reset();
        myApp->OnExit();
        delete myApp;
        wxEntryCleanup();
    }

    void updateTimerCount() {
        timers = con->getTimers().size();
    }

    void updateThreadCount() {
        threads = con->getThreads().size();
    }
};

TEST_F(controllerTest, Timers) {
    updateTimerCount();
    con->createNewTimer();
    ASSERT_EQ(con->getTimers().size(), timers + 1);

    con->deleteSelectedTimer();
    ASSERT_EQ(con->getTimers().size(), timers);
}

TEST_F(controllerTest, Threads) {
    updateThreadCount();
    con->createNewTimer();
    con->startSelectedTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(myApp->c->getSelectedTimer()->isRunning(), true);
    EXPECT_EQ(myApp->c->getThreads().size(), threads + 1);

    con->stopSelectedTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(myApp->c->getSelectedTimer()->isRunning(), false);
    EXPECT_EQ(myApp->c->getThreads().size(), threads);
}