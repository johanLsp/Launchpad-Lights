cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(lights LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_definitions("-Wno-psabi")

FIND_PACKAGE( Boost 1.40 COMPONENTS system chrono REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package (Threads)
find_library(wiringPi_LIB wiringPi)

find_package(czmq)
include_directories(${CZMQ_INCLUDE_DIRS})

include(lib/AbletonLink/AbletonLinkConfig.cmake)

include_directories(lib/json)
add_subdirectory(client/midi)
add_subdirectory(src)

