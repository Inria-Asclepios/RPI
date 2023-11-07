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


cmake_minimum_required(VERSION 3.24)
set(CMAKE_MACOSX_RPATH OFF)

set(RPI_VERSION_NUMBER 1.2.0)
mark_as_advanced(RPI_VERSION_NUMBER)

project(RPI VERSION ${RPI_VERSION_NUMBER})

###############################################################################
# INCLUDES SOME CMAKE HELPERS
###############################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(export_and_install_libs)
include(ITKLibs)
include(FetchTCLAP)

###############################################################################
# FETCH TCLAP
###############################################################################
#FetchContent_Declare(
#  tclap
#  URL https://netix.dl.sourceforge.net/project/tclap/tclap-1.2.1.tar.gz
#  URL_HASH MD5=eb0521d029bf3b1cc0dcaa7e42abf82a
#)
#FetchContent_MakeAvailable(tclap)
#set(TCLAP_INCLUDEDIR ${CMAKE_BINARY_DIR}/_deps/tclap-src/include)
#set(TCLAP_DIR ${CMAKE_BINARY_DIR}/_deps/tclap-src/include)
#include_directories(${CMAKE_BINARY_DIR}/_deps/tclap-src/include)


###############################################################################
# Compilation
###############################################################################
add_subdirectory(src)


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
#include(RPIPack)
