# Target sources and build options

set(TARGET_SOURCES
        main.cpp
        focusdiagnosticsmainwindow.cpp
        focusdiagnosticsmainwindow.h
        focusdiagnosticsmainwindow.ui
)

set(TARGET_NAME FocusDiagnostics)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTFastPlotter6
        JKQTPlotter6::JKQTPlotter6
        ${ROOT_LIBRARIES}
)

include(${PROJECT_SOURCE_DIR}/ExecutableTarget.cmake)
