cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(lights LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_definitions("-Wno-psabi")

FIND_PACKAGE( Boost 1.40 COMPONENTS system chrono REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package (Threads)
find_library(wiringPi_LIB wiringPi)


include(lib/AbletonLink/AbletonLinkConfig.cmake)

include_directories(include)
add_subdirectory(src)

