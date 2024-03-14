cmake_minimum_required(VERSION 3.5)

project(TestingQWT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)

include_directories(/usr/local/qwt-6.2.0/include)
find_library(QWT NAMES qwt PATHS /usr/local/qwt-6.2.0/lib REQUIRED)

set(JKQTCommon6_DIR      "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTMath6_DIR        "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTMathText6_DIR    "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTFastPlotter6_DIR "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTPlotter6_DIR     "/usr/local/lib/cmake/JKQTPlotter6")
find_package(JKQTCommon6        REQUIRED)
find_package(JKQTMathText6      REQUIRED)
find_package(JKQTFastPlotter6   REQUIRED)
find_package(JKQTPlotter6       REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TestingQWT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TestingQWT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TestingQWT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TestingQWT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_directories(TestingQWT PRIVATE /usr/local/lib)
target_link_libraries(TestingQWT PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::OpenGL
    JKQTCommon6
    JKQTMathText6
    JKQTMath6
    JKQTFastPlotter6
    JKQTPlotter6
)

set_target_properties(TestingQWT PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TestingQWT)
endif()
