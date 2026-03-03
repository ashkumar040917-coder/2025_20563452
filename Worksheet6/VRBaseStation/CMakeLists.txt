cmake_minimum_required(VERSION 3.19)
project(VRBaseStation LANGUAGES CXX)

# --- PHASE 0 & 3: Enable Qt Automatic Tools ---
# These are essential for Signals/Slots, UI files, and Resource files [cite: 300]
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt6 packages [cite: 20]
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# --- PHASE 5: Exercise 4 - Define all project files ---
# All .cpp, .h, and .ui files must be listed here to be compiled [cite: 362, 677]
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    optiondialog.cpp
    optiondialog.h
    optiondialog.ui
)

# Create the executable using the consolidated source list
qt_add_executable(VRBaseStation
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
    icons.qrc
    optiondialog.h optiondialog.cpp optiondialog.ui
)

# --- PHASE A: Link Libraries ---
# Links your code to the Qt framework [cite: 20]
target_link_libraries(VRBaseStation
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# --- Installation and Deployment Logic ---
include(GNUInstallDirs)

install(TARGETS VRBaseStation
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET VRBaseStation
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
