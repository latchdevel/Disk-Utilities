# CMake script to generate build files for "disk-analyse" tool
# 
# Copyright (c) 2022 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.14)

# Executable "disk-analyse" in C language
project(disk-analyse C)

# Add libdisk library directory, excluding unused "disk-dynamic" target
add_subdirectory( ../libdisk/ libdisk EXCLUDE_FROM_ALL )

# Check if has parent directory
get_directory_property(hasParent PARENT_DIRECTORY)

# Set C Standard
set(CMAKE_C_STANDARD 99)

# Set C FLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -fcommon -fno-strict-aliasing -fvisibility=hidden")

# if macOS build a Mach-O universal binary with 2 architectures: x86_64 and arm64 for M1 processors
if (APPLE) 
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

# Add main source files
FILE ( GLOB SRC *.c )

# Add executable target
add_executable( ${PROJECT_NAME} ${SRC} )

# Basic check for PREFIX
if(PREFIX)
    MESSAGE ( STATUS "Set PREFIX: ${PREFIX}" ) 
    set ( CMAKE_INSTALL_PREFIX ${PREFIX} ) 
else()
    set(PREFIX ${CMAKE_INSTALL_PREFIX})
    MESSAGE ( STATUS "Default PREFIX: ${PREFIX}" ) 
endif()

# Set PREFIX required by config.c
target_compile_definitions( ${PROJECT_NAME} PRIVATE PREFIX="${PREFIX}" )

# Add include directory
target_include_directories( ${PROJECT_NAME} PRIVATE ../libdisk/include )

# Basic check for BSD based systems, like as FreeBSD, NetBSD, OpenBSD, etc.
string( TOUPPER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_UPPER )
if(NOT CMAKE_SYSTEM_NAME_UPPER MATCHES "BSD")
  # If not a BSD system, add executable export symbols for loadable modules. Add linker flag "-Wl,--export-dynamic"
  set_target_properties( ${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE ) 
endif()

# Build static executable if possible
if( NOT (CMAKE_SYSTEM_NAME MATCHES "Darwin" OR MSVC) )
  set( CMAKE_EXE_LINKER_FLAGS "-static" )
endif()

# Add link to libdisk library target: "disk" for static link (instead "disk-dynamic")
target_link_libraries( ${PROJECT_NAME} PRIVATE disk )

# Add install executable target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Add install formats file target
install(FILES formats DESTINATION share/disk-analyse )

# If no has parent directory, add uninstall targets
if(NOT hasParent)
  add_custom_target( uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
  )
endif()
