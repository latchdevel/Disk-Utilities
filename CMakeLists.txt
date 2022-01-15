# CMake script to generate build files for "Disk-Utilities"
# 
# Copyright (c) 2022 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.14)

# Main proyect name
project(Disk-Utilities)

# Add directory for disk-analyse executable
add_subdirectory( disk-analyse/ )

add_custom_target( uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
)
