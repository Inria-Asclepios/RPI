###############################################################################
# RPI
# Authors: F.Leray
# Created: 07/11/2023 
#
# Distributed under the BSD licence:
# Copyright (c) 2023, INRIA
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

# Try to find or fetch the TCLAP library
# Once done this will define
#
# TCLAP_FOUND         - system has TCLAP and it can be used
# TCLAP_INCLUDE_DIR   - directory where the header file can be found
#

option(TCLAP_FETCH "Fetch TCLAP directly on internet" ON)
SET( TCLAP_FOUND FALSE )
FIND_PATH( TCLAP_INCLUDE_DIR tclap/CmdLine.h ${TCLAP_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/_deps/tclap-src/include tclap/CmdLine.h /usr/include /opt/local/include )
SET(TCLAP_INCLUDE_DIR "${TCLAP_INCLUDE_DIR}" CACHE PATH "" FORCE)

IF( TCLAP_INCLUDE_DIR )
    SET( TCLAP_FOUND TRUE )
ELSE() 
    IF( ${TCLAP_FETCH} )        
        include(FetchContent)
        FetchContent_Declare(
            tclap
            URL https://netix.dl.sourceforge.net/project/tclap/tclap-1.2.1.tar.gz
            URL_HASH MD5=eb0521d029bf3b1cc0dcaa7e42abf82a
            )            
    FetchContent_MakeAvailable(tclap)
    SET(TCLAP_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/tclap-src/include CACHE PATH "" FORCE)
    SET( TCLAP_FOUND TRUE )
    ELSE()
       MESSAGE( FATAL_ERROR "TCLAP was not found." )     
    ENDIF()   
ENDIF()

IF( TCLAP_FOUND )
    include_directories(${TCLAP_INCLUDE_DIR})
ENDIF()
