# CMake script to generate build files for "libdisk" library
# 
# Copyright (c) 2022 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.14)

# Library "libdisk" in C language
project(disk C)

# Check if has parent directory
get_directory_property(hasParent PARENT_DIRECTORY)

# Set version numbers 
set( MAJOR_VERSION 0 ) # MAYOR
set( MINOR_VERSION 0 ) # MINOR

# Set version number for shared libraries and executables
set(CU_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}) # current version
set(SO_VERSION ${MAJOR_VERSION} )                 # compatibility version

# Set C Standard
set(CMAKE_C_STANDARD 99)

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

# Show build type if not has parent directory
if(NOT hasParent)
    MESSAGE( STATUS "Build type set to: " ${CMAKE_BUILD_TYPE} )
endif()

# Set "debug" and "release" specific C compiler flags
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")                      # Default "-O3 -DNDEBUG"
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g -DDEBUG -fsanitize=address") # Default "-g"

# Set common C compiler flags
set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -fcommon -fno-strict-aliasing -fvisibility=hidden")

# Add main library sources
FILE (GLOB ${PROJECT_NAME}_SRC *.c )

# Add container sources 
FILE (GLOB CONTAINER container/*.c )

# Add format sources
FILE (GLOB_RECURSE FORMAT format/*.c )

# Add scream sources
FILE (GLOB STREAM stream/*.c )

# Basic check for CAPS/IPF stream support
if(CAPS)
    STRING( TOLOWER "${CAPS}" CAPS )
    if( (CAPS STREQUAL "y") OR (CAPS STREQUAL "yes"))
        set(CAPS "y")
    endif()
else()
    set(CAPS "n")
endif()

# Select caps source file
if(CAPS STREQUAL "y")
    MESSAGE( STATUS "CAPS/IPF support enabled")
    get_filename_component(caps_full_path stream/caps_disabled.c ABSOLUTE)
    list(REMOVE_ITEM STREAM ${caps_full_path})
else()
    MESSAGE( STATUS "CAPS/IPF support disabled")
    get_filename_component(caps_full_path stream/caps.c ABSOLUTE)
    list(REMOVE_ITEM STREAM ${caps_full_path})
endif()

# if macOS build a Mach-O universal binary with 2 architectures: x86_64 and arm64 for M1 processors
if (APPLE) 
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

# Compile library as objects
add_library( 
                ${PROJECT_NAME}-obj OBJECT
                ${${PROJECT_NAME}_SRC}
                ${STREAM}
                ${CONTAINER} 
                ${FORMAT}
)

# Add include directory
target_include_directories( ${PROJECT_NAME}-obj PRIVATE include/ )

# Shared libraries require compiles for "Independent Position Code" (PIC). Add compiler flag "-fPIC"
set_target_properties( ${PROJECT_NAME}-obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE )

# Add static library built from object files 
# File extension OS depends, like: .a or .lib
add_library( 
                ${PROJECT_NAME} STATIC 
                $<TARGET_OBJECTS:${PROJECT_NAME}-obj>
)

# Add dynamic libray built from object files 
add_library( 
                ${PROJECT_NAME}-dynamic SHARED 
                $<TARGET_OBJECTS:${PROJECT_NAME}-obj>
)

# File extension OS depends, like: .so or .dylib or .dll
set_target_properties( ${PROJECT_NAME}-dynamic PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )

# Set version numbers for the versioned shared libraries target.
# For shared libraries and executables on Windows and Mach-O systems 
# the SOVERSION property corresponds to the compatibility version 
# and VERSION corresponds to the current version
# 
# Note that SOVERSION will still be used to form the install_name and 
# both SOVERSION and VERSION may also affect the file and symlink names.
# Use the NAMELINK_SKIP option of the install command to prevent the 
# generation of the versionless library name symbolic link to the 
# versioned library file.
set_target_properties( ${PROJECT_NAME}-dynamic PROPERTIES
		SOVERSION ${SO_VERSION}
		VERSION   ${CU_VERSION}
)

# Add install static library and public headers
FILE ( GLOB LIBRARY_HEADERS include/libdisk/*.h )
set_target_properties( ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${LIBRARY_HEADERS}")
install( TARGETS ${PROJECT_NAME} 
         LIBRARY DESTINATION lib
         PUBLIC_HEADER DESTINATION include/libdisk
)

# Add install dynamic library (with namelink)
install( TARGETS ${PROJECT_NAME}-dynamic DESTINATION lib )

# If no has parent directory, add uninstall targets
if(NOT hasParent)
  MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
  add_custom_target( uninstall
      "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
  )
endif()
