cmake_minimum_required(VERSION 3.25)

project(Timer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

add_library(core
    src/App.cpp
    src/App.h
    src/Controller.cpp
    src/Controller.h
    src/ControllerInterface.h
    src/Date.cpp
    src/Date.h
    src/FormatInfoDialog.cpp
    src/FormatInfoDialog.h
    src/Frame.cpp
    src/Frame.h
    src/OptionsFrame.cpp
    src/OptionsFrame.h
    src/Timer.cpp
    src/Timer.h
)
target_compile_features(core PUBLIC cxx_std_20)
target_link_libraries(core PUBLIC ${wxWidgets_LIBRARIES})

add_executable(Timer src/main.cpp)
target_link_libraries(Timer PRIVATE core)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
