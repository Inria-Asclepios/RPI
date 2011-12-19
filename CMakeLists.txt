###############################################################################
# RPI
# Authors: B.Bleuzé, V.Garcia
# Created: 04/04/2011 
#
# Distributed under the BSD licence:
# Copyright (c) 2011, INRIA
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, 
# this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
# - Neither the name of INRIA nor the names of its contributors may be used 
# to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
# PURPOSE ARE DISCLAIMED. 
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
###############################################################################


cmake_minimum_required(VERSION 2.6)
project(RPI)

set(${PROJECT_NAME}_VERSION_MAJOR 1 CACHE STRING "RPI major version number.")
set(${PROJECT_NAME}_VERSION_MINOR 2 CACHE STRING "RPI minor version number.")
set(${PROJECT_NAME}_VERSION_BUILD 0 CACHE STRING "RPI build version number.")
set(${PROJECT_NAME}_VERSION
  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD}")

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(BUILD_SHARED_LIBS   "Build shared libraries" true)

set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)
set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)
set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY lib)


# Set runtime, library, and archive output directories
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY}  CACHE PATH "Single Directory for all executables"       )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY}  CACHE PATH "Single Directory for all dynamic libraries" )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY}  CACHE PATH "Single Directory for all static libraries"  )

set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/RegistrationProgrammingInterface" "${CMAKE_SOURCE_DIR}/examples")

# Set runtime, library, and archive install output directories
set ( RUNTIME_INSTALL_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY}  CACHE PATH "Single Install Directory for all executables"       )
set ( LIBRARY_INSTALL_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY}  CACHE PATH "Single Install Directory for all dynamic libraries" )
set ( ARCHIVE_INSTALL_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY}  CACHE PATH "Single Install Directory for all static libraries"  )


set(${PROJECT_NAME}_LIBRARY_DIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(${PROJECT_NAME}_CMAKE_DIRS "${CMAKE_SOURCE_DIR}/cmake")
set(${PROJECT_NAME}_USE_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Use.cmake")


set(${PROJECT_NAME}_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include) 
set(${PROJECT_NAME}_INSTALL_LIBRARY_DIRS ${LIBRARY_INSTALL_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_INSTALL_RUNTIME_DIRS ${RUNTIME_INSTALL_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_INSTALL_CMAKE_DIRS ${CMAKE_INSTALL_PREFIX}/cmake)
set(${PROJECT_NAME}_INSTALL_USE_FILE ${CMAKE_INSTALL_PREFIX}/cmake/${PROJECT_NAME}Use.cmake)

#Sub projects:
option(RPI_BUILD_EXAMPLES  "Build Examples" "OFF")

if (RPI_BUILD_EXAMPLES)
       add_subdirectory(examples)
endif(RPI_BUILD_EXAMPLES)

add_subdirectory(RegistrationProgrammingInterface)

##################################################################
# Setup use file
##################################################################

if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in)
configure_file( ## Common use file
  ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in
  ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Use.cmake
  @ONLY IMMEDIATE)
endif(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in)



###############################################################################
# Setup Configuration files
###############################################################################

if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in)
configure_file( ## Build tree configure file
  ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  @ONLY IMMEDIATE)
endif(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in)

if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.install.cmake.in)
configure_file( ## Install tree configure file
  ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.install.cmake.in
  ${${PROJECT_NAME}_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
  @ONLY IMMEDIATE)
endif(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.install.cmake.in)


## #################################################################
## Install cmake files
## #################################################################

if( EXISTS ${${PROJECT_NAME}_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
AND EXISTS ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Use.cmake)
#AND EXISTS ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Uninstall.cmake)

install(FILES
  ${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake
# ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Uninstall.cmake
  DESTINATION
  cmake)

endif()


###############################################################################
# Documentation
###############################################################################

# Create doxygen documentation
option (BUILD_DOCUMENTATION OFF)
if (BUILD_DOCUMENTATION)
    INCLUDE ( UseDoxygen )
    set (DOXYFILE_LATEX "NO")
endif()


###############################################################################
# Package
###############################################################################
include(RPIPack)
