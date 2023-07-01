cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME Timer)
project(${PROJECT_NAME})

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set(SOURCE_FILES src/Frame.cpp src/Frame.h src/App.cpp src/App.h src/Date.cpp src/Date.h src/Timer.cpp src/Timer.h src/OptionsFrame.cpp src/OptionsFrame.h src/Controller.cpp src/Controller.h src/Observer.h src/FormatInfoDialog.cpp src/FormatInfoDialog.h)
add_executable(${PROJECT_NAME} src/main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} core ${wxWidgets_LIBRARIES})

add_subdirectory(tests)
