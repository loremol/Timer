find_package(GTest REQUIRED)

add_executable(Tests
    ControllerTest.cpp
    DateTest.cpp
    runAllTests.cpp
    TimerTest.cpp
)
target_link_libraries(Tests PRIVATE core GTest::gtest)

include(GoogleTest)
gtest_discover_tests(Tests)
