cmake_minimum_required(VERSION 3.9)
project(clash_qt)

set(CMAKE_CXX_STANDARD 17)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

include(FeatureSummary)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(yaml-cpp CONFIG REQUIRED)


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

include_directories(src)

set(src_files
        src/main.cpp
        src/ui/window/mainwindow.cpp src/ui/window/mainwindow.h
        src/clash/clash.cpp src/clash/clash.h src/clash/restfulapi.cpp src/clash/restfulapi.h
        src/ui/widget/proxywidget.h src/ui/widget/proxywidget.cpp
        src/ui/widget/proxygroupwidget.h src/ui/widget/proxygroupwidget.cpp
        src/ui/window/proxypage.h src/ui/window/proxypage.cpp
        src/ui/util/flowlayout.cpp src/ui/util/flowlayout.h)

set(res_files res/res.qrc)
add_executable(clash-qt ${src_files} ${res_files})

target_link_libraries(clash-qt
        Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network
        yaml-cpp
        )