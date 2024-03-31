# DEPENDENCIES

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

## CERN ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

## TinyTIFF
find_package(TIFF REQUIRED)

# Target sources and build options
include_directories(/usr/include/gstreamer-1.0 /usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include)

set(TARGET_SOURCES
        main.cpp
        focusdiagnosticsmainwindow.cpp
        focusdiagnosticsmainwindow.h
        focusdiagnosticsmainwindow.ui
        ImageDataModel.h
        ImageDataModel.cpp
        Controller.h
        Controller.cpp
        ISCameraHelpers.h
        ISCameraController.h
        ISCameraController.cpp
)

set(TARGET_NAME FocusDiagnostics)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTPlotter6
        ${ROOT_LIBRARIES} Core Hist Tree MathCore ASImage
        TIFF::TIFF
        tcam
        tcam-property
        tcamgststatistics
        tcam-network
        gobject-2.0
        glib-2.0
        gstreamer-1.0
        gstvideo-1.0
)

qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${TARGET_SOURCES} resources.qrc)
target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_DEPENDENCIES})
qt_finalize_executable(${TARGET_NAME})
