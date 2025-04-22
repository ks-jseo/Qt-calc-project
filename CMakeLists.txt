cmake_minimum_required(VERSION 3.14)

project(QtCalculator)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Widgets Qml)

add_executable(QtCalculator src/main.cpp)

target_link_libraries(QtCalculator Qt6::Widgets Qt6::Qml)