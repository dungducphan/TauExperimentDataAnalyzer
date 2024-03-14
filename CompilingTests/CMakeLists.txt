# Target sources and build options

set(TARGET_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

set(TARGET_NAME CompilingTests)

set(TARGET_DEPENDENCIES
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::OpenGL
        JKQTPlotter6::JKQTCommon6
        JKQTPlotter6::JKQTMathText6
        JKQTPlotter6::JKQTMath6
        JKQTPlotter6::JKQTFastPlotter6
        JKQTPlotter6::JKQTPlotter6
        ${ROOT_LIBRARIES}
)

include(${PROJECT_SOURCE_DIR}/ExecutableTarget.cmake)
