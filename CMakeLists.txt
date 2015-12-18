cmake_minimum_required(VERSION 3.0)

# Project name.
set(PROJECT qcron)
project(${PROJECT})

set(SOURCE_DIR         ${CMAKE_SOURCE_DIR}/src)
set(ARCHIVE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib)
set(PUBLIC_HEADER_DIR  ${CMAKE_SOURCE_DIR}/include)

if (WIN32)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
endif()

set(${PROJECT}_INCLUDE_DIR
  ${SOURCE_DIR} CACHE STRING "${PROJECT} include directory")

# -Wmissing-declarations delete because it would not make Qt Resource
# Files compiles
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls \
-Wcast-align -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
-Wextra -Wall -Werror -Winvalid-pch -Wredundant-decls -Wformat=2 \
-Wmissing-format-attribute -Wformat-nonliteral")

option(WITH_UNIT_TESTS "enable building unit test executable" OFF)
#option(WITH_BENCHMARK "enable building benchmark executable" OFF)

# Dependencies
set(QT_COMPONENTS Core Test)

## Qt5
if (WIN32)
  ### Required because on Windows, FindQt5.cmake is not always available.
  set (CMAKE_PREFIX_PATH ${QT_INSTALL_PATH})
endif()
### Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
### Find the Qt components.
find_package(Qt5 COMPONENTS ${QT_COMPONENTS} REQUIRED)

# Source
add_subdirectory(src)

if(WITH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
