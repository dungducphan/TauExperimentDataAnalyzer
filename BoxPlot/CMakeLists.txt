# Target sources and build options

set(TARGET_SOURCES
        main.cpp
)

set(TARGET_NAME BoxPlot)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTPlotter6
)

include(${PROJECT_SOURCE_DIR}/ExecutableTarget.cmake)
