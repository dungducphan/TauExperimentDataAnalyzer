cmake_minimum_required(VERSION 3.5)

project(TauExperimentDataAnalyzer)

project(TestingQWT VERSION 0.1 LANGUAGES CXX)

# Common CMake settings

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies

find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)

include_directories(/usr/local/qwt-6.3.0-dev/include)
find_library(QWT NAMES qwt PATHS /usr/local/qwt-6.3.0-dev/lib REQUIRED)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

set(JKQTCommon6_DIR      "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTMath6_DIR        "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTMathText6_DIR    "/usr/local/lib/cmake/JKQTPlotter6")
set(JKQTPlotter6_DIR     "/usr/local/lib/cmake/JKQTPlotter6")
find_package(JKQTCommon6        REQUIRED)
find_package(JKQTMathText6      REQUIRED)
find_package(JKQTMath6          REQUIRED)
find_package(JKQTPlotter6       REQUIRED)

# Applications

add_subdirectory(CompilingTests)
add_subdirectory(BoxPlot)
add_subdirectory(FocusDiagnostics)
