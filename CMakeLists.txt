# CMake script to generate build files for "Disk-Utilities"
# 
# Copyright (c) 2022 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.14)

# Main proyect name
project(Disk-Utilities)

# Set default build type to "release".
set(DEFAULT_BUILD_TYPE "release")

# Checking for CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
  # Set default build type
  set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
else()
  # Check if set a valid CMAKE_BUILD_TYPE ('release' or 'debug')
  STRING( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )
  if(NOT((CMAKE_BUILD_TYPE STREQUAL "debug") OR (CMAKE_BUILD_TYPE STREQUAL "release")))
    # Exit error
    MESSAGE( FATAL_ERROR "If set CMAKE_BUILD_TYPE it must be 'release' or 'debug'")
  endif()
endif()

# Show build type
MESSAGE( STATUS "Build type set to: " ${CMAKE_BUILD_TYPE} )

# Add directory for disk-analyse executable
add_subdirectory( disk-analyse/ )

add_custom_target( uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
)
