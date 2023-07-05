cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Tests)
project(${PROJECT_NAME})

find_package(GTest REQUIRED)
find_package(wxWidgets)
include(${wxWidgets_USE_FILE})


set(SOURCE_FILES runAllTests.cpp DateTest.cpp TimerTest.cpp ControllerTest.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} core ${wxWidgets_LIBRARIES} ${GTEST_BOTH_LIBRARIES})