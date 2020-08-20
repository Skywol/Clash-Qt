cmake_minimum_required(VERSION 3.9)
project(clash_qt)

set(CMAKE_CXX_STANDARD 11)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

include(FeatureSummary)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 5.12.0 REQUIRED COMPONENTS Core Gui Widgets)
find_package(KF5 REQUIRED COMPONENTS
        WidgetsAddons
        )


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(src_files
        src/main.cpp
        src/ui/widget/collapsiblearea.cpp src/ui/widget/collapsiblearea.h src/ui/window/mainwindow.cpp src/ui/window/mainwindow.h)

set(res_files res/res.qrc)
add_executable(clash-qt ${src_files} ${res_files})

target_link_libraries(clash-qt
        Qt5::Core Qt5::Gui Qt5::Widgets
        KF5::WidgetsAddons
        yaml-cpp)