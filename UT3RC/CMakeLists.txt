########################################################################################################################
# DEPENDENCIES
########################################################################################################################

### JKQtPlotter paths
set(JKQTPLOTTER_CMAKE_MODULE_DIR /usr/local/lib/cmake/JKQTPlotter6)
set(JKQTCommon6_DIR      ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTMath6_DIR        ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTMathText6_DIR    ${JKQTPLOTTER_CMAKE_MODULE_DIR})
set(JKQTPlotter6_DIR     ${JKQTPLOTTER_CMAKE_MODULE_DIR})

## JKQtPlotter
find_package(JKQTCommon6        REQUIRED)
find_package(JKQTMathText6      REQUIRED)
find_package(JKQTMath6          REQUIRED)
find_package(JKQTPlotter6       REQUIRED)

## Qt6 modules
find_package(QT NAMES Qt6 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

## ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

## TinyTIFF
find_package(TIFF REQUIRED)

## EPICS
set(EPICS_BASE_DIR /home/ubuntu/Applications/epics/epics-base-install)
set(EPICS_LIBRARY_DIR ${EPICS_BASE_DIR}/lib/linux-x86_64)
set(EPICS_INCLUDE_DIR
        ${EPICS_BASE_DIR}/include
        ${EPICS_BASE_DIR}/include/compiler/gcc
        ${EPICS_BASE_DIR}/include/os/Linux
)

include_directories(${EPICS_INCLUDE_DIR})
find_library(EPICS7_Com        NAMES Com        PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_CA         NAMES ca         PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVAccess   NAMES pvAccess   PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVData     NAMES pvData     PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVDatabase NAMES pvDatabase PATHS ${EPICS_LIBRARY_DIR} REQUIRED)
find_library(EPICS7_PVAClient  NAMES pvaClient  PATHS ${EPICS_LIBRARY_DIR} REQUIRED)

## QE
set(QEFRAMEWORK_BASE_DIR /home/ubuntu/Applications/qtepics/qeframework)
set(QEFRAMEWORK_LIBRARY_DIR ${QEFRAMEWORK_BASE_DIR}/lib/linux-x86_64)
set(QEFRAMEWORK_INCLUDE_DIR ${QEFRAMEWORK_BASE_DIR}/include)

########################################################################################################################
# TARGET
########################################################################################################################
set(TARGET_SOURCES
        main.cpp
)

set(TARGET_NAME UT3RC)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTPlotter6
        ${ROOT_LIBRARIES} Core Hist Tree MathCore ASImage
        TIFF::TIFF
        QEFramework
)

qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${TARGET_SOURCES} resources.qrc)
target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_DEPENDENCIES})
qt_finalize_executable(${TARGET_NAME})
