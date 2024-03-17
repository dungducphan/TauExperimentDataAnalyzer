cmake_minimum_required(VERSION 3.5)

project(TauExperimentDataAnalyzer VERSION 0.1 LANGUAGES CXX)

#----------------------------------------------------------------------------------------------------------------------#
# Common settings
#----------------------------------------------------------------------------------------------------------------------#

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## User settings

### QWT paths
set(QWT_INCLUDE_DIR /usr/local/qwt-6.3.0-dev/include)
set(QWT_LIBRARY_DIR /usr/local/qwt-6.3.0-dev/lib)

### JKQtPlotter paths
set(JKQTPLOTTER_CMAKE_MODULE_DIR /usr/local/lib/cmake/JKQTPlotter6)
set(JKQTCommon6_DIR      ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTMath6_DIR        ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTMathText6_DIR    ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTPlotter6_DIR     ${JKQTPLOTTER_CMAKE_MODULE_DIR})

# EPICS paths
set(EPICS_BASE_DIR /home/ubuntu/Applications/epics/epics-base-install)
set(EPICS_LIBRARY_DIR ${EPICS_BASE_DIR}/lib/linux-x86_64)
set(EPICS_INCLUDE_DIR
        ${EPICS_BASE_DIR}/include
        ${EPICS_BASE_DIR}/include/compiler/gcc
        ${EPICS_BASE_DIR}/include/os/Linux
)

#----------------------------------------------------------------------------------------------------------------------#
# Dependencies
#----------------------------------------------------------------------------------------------------------------------#

## Qt6 modules
find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL Charts REQUIRED)

## Qwt
include_directories(${QWT_INCLUDE_DIR})
find_library(QWT NAMES qwt PATHS ${QWT_LIBRARY_DIR} REQUIRED)

# CERN ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

## JKQtPlotter
find_package(JKQTCommon6        REQUIRED)
find_package(JKQTMathText6      REQUIRED)
find_package(JKQTMath6          REQUIRED)
find_package(JKQTPlotter6       REQUIRED)

## EPICS
include_directories(${EPICS_INCLUDE_DIR})
find_library(EPICS7_Com        NAMES Com        PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_CA         NAMES ca         PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVAccess   NAMES pvAccess   PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVData     NAMES pvData     PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVDatabase NAMES pvDatabase PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVAClient  NAMES pvaClient  PATHS ${EPICS_LIBRARY_DIR} REQUIRED)

# TinyTIFF
find_package(TIFF REQUIRED)

#----------------------------------------------------------------------------------------------------------------------#
# Applications
#----------------------------------------------------------------------------------------------------------------------#

add_subdirectory(CompilingTests)
add_subdirectory(FocusDiagnostics)
