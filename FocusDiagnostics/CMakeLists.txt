# Target sources and build options

set(TARGET_SOURCES
        main.cpp
        focusdiagnosticsmainwindow.cpp
        focusdiagnosticsmainwindow.h
        focusdiagnosticsmainwindow.ui
        ImageDataModel.h
        ImageDataModel.cpp
        Controller.h
        Controller.cpp
)

set(TARGET_NAME FocusDiagnostics)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTPlotter6
        ${QWT}
        ${ROOT_LIBRARIES} Core Hist Tree MathCore ASImage
        ${EPICS7_PVAccess}
        ${EPICS7_PVData}
        ${EPICS7_Com}
        ${EPICS7_CA}
        TIFF::TIFF
)

qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${TARGET_SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_DEPENDENCIES})
qt_finalize_executable(${TARGET_NAME})