cmake_minimum_required(VERSION 3.16)

add_executable(cluster
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

target_link_libraries(cluster_ui
    PRIVATE
        Qt6::Widgets
        can_interface
)
