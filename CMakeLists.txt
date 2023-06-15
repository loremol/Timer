cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Timer)
project(${PROJECT_NAME})

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set(SOURCE_FILES src/MainFrame.cpp src/MainFrame.h src/App.cpp src/App.h src/Date.cpp src/Date.h)
add_executable(${PROJECT_NAME} src/main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(core ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} core)

add_subdirectory(tests)
