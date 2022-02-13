# ----- PROJECT BUILD CONFIG -----
cmake_minimum_required(VERSION 3.22)
project(VecFX LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
add_subdirectory(dependencies)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(pugixml REQUIRED)
find_package(resvg REQUIRED)

add_executable(VecFX
    src/csscolorparser.cpp
    src/csscolorparser.hpp
    src/kfTableModel.cpp
    src/kfTableModel.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/resvg.h
    src/ResvgQt.h
    src/SVGObject.cpp
    src/SVGObject.hpp
    src/mainwindow.ui
)
target_link_libraries(VecFX PRIVATE Qt5::Widgets pugixml resvg)

