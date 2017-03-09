cmake_minimum_required(VERSION 2.8.3)
project(EPSON-Robot)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system)
find_package(Qt5Network REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories( include)

## Declare a C++ library

set(LIBRARY_SOURCES
        src/filesender.cpp
        )

add_library(filesenderlib ${LIBRARY_SOURCES})
target_link_libraries(filesenderlib
        Qt5::Core
        Qt5::Network)
set(SOURCE_FILES
        src/filesender.cpp
        src/test.cpp
        )

add_executable(test_node ${SOURCE_FILES})
target_link_libraries(test_node
        Qt5::Core
        Qt5::Network)

