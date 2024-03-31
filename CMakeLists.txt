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

#----------------------------------------------------------------------------------------------------------------------#
# Applications
#----------------------------------------------------------------------------------------------------------------------#

add_subdirectory(UT3RC)
add_subdirectory(FocusDiagnostics)
