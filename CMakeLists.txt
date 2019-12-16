cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(lights LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

SET(BUILD_CLIENT_ONLY NO CACHE STRING "Build only the client application")

add_definitions("-Wno-psabi")

# CZMQ and RTMidi required by both the client and the main application.
find_package(czmq)
include_directories(${CZMQ_INCLUDE_DIRS})
find_package(RTMidi)
include_directories(${RTMIDI_INCLUDE_DIRS})

if (NOT BUILD_CLIENT_ONLY)
  find_package(Boost 1.64 COMPONENTS system filesystem chrono REQUIRED)
  include_directories(${Boost_INCLUDE_DIR})
  find_package(Threads)
  find_library(wiringPi_LIB wiringPi)
  include(lib/AbletonLink/AbletonLinkConfig.cmake)
  include_directories(lib/json)

  add_subdirectory(src)

endif()

add_subdirectory(client/midi)

