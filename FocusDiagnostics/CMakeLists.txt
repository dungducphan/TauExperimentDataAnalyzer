# Target sources and build options

set(TARGET_SOURCES
        main.cpp
        GUI/focusdiagnosticsmainwindow.cpp
        GUI/focusdiagnosticsmainwindow.h
        GUI/focusdiagnosticsmainwindow.ui
)

set(TARGET_NAME FocusDiagnostics)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
#        JKQTPlotter6::JKQTCommon6
#        JKQTPlotter6::JKQTMathText6
#        JKQTPlotter6::JKQTMath6
#        JKQTPlotter6::JKQTPlotter6
#        ${QWT}
#        ${ROOT_LIBRARIES}
        ${EPICS7_PVAccess}
        ${EPICS7_PVData}
        ${EPICS7_Com}
        ${EPICS7_CA}
)

include(${PROJECT_SOURCE_DIR}/ExecutableTarget.cmake)
