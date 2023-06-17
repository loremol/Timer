cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Tests)
project(${PROJECT_NAME})

find_package(GTest REQUIRED)

set(SOURCE_FILES runAllTests.cpp DateTest.cpp TimerTest.cpp)
add_executable(runAllTests ${SOURCE_FILES})
target_link_libraries(runAllTests ${GTEST_BOTH_LIBRARIES} core)