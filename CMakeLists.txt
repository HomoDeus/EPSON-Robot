cmake_minimum_required(VERSION 2.8.3)
project(EPSON-Robot)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system)
find_package(Qt5Network REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories( include)

#QT headers needed to be processed with QT MOH compiler
SET(moh_hdrs
        include/RobotDriver.h
        include/client_console.h
        include/server.h
        )
#Generate CPP files from moh headers
QT5_WRAP_CPP(moh_srcs ${moh_hdrs})

## Declare a C++ library

set(LIBRARY_SOURCES
        src/filesender.cpp
        src/RobotDriver.cpp
        src/client_console.cpp
        src/server.cpp
        ${moh_srcs})



add_library(filesenderlib ${LIBRARY_SOURCES})
target_link_libraries(filesenderlib
        Qt5::Core
        Qt5::Network
        Qt5::Widgets)


set(SOURCE_FILES
        ${LIBRARY_SOURCES}
        src/clienttest.cpp)

add_executable(test_node ${SOURCE_FILES})
target_link_libraries(test_node
        Qt5::Core
        Qt5::Network
        Qt5::Widgets)

set(SOURCE_FILES
        ${LIBRARY_SOURCES}
        src/servertest.cpp)

add_executable(servertest_node ${SOURCE_FILES})
target_link_libraries(servertest_node
        Qt5::Core
        Qt5::Network
        Qt5::Widgets)