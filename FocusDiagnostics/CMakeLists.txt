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
        SpeedTestPlot.h
        SpeedTestPlot.cpp
)

set(TARGET_NAME FocusDiagnostics)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTPlotter6
        ${ROOT_LIBRARIES} Core Hist Tree MathCore ASImage
        ${EPICS7_PVAccess}
        ${EPICS7_PVData}
        ${EPICS7_Com}
        ${EPICS7_CA}
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
