cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(CarEntertainmentClusterUnit
    VERSION 0.1.0
    LANGUAGES C CXX
)

add_subdirectory(core)
add_subdirectory(apps)
