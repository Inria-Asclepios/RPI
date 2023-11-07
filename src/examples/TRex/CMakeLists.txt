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


# Project name
PROJECT( TREX )

# Define the minimum CMake version needed
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )


# Check if ITK is found and include useful files
FIND_PACKAGE( ITK )
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )


# Check if TCLAP is found and include directory
find_package (TCLAP REQUIRED)

IF( NOT TCLAP_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires TCLAP and TCLAP was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE_DIRECTORIES( ${TCLAP_INCLUDE_DIR} )



# Set used libraries
SET(LIBRARIES
    ${ITKIO_LIBRARIES}
    ${ITK_TRANSFORM_LIBRARIES}	
    ${ITKIOPhilipsREC_LIBRARIES}
    ITKOptimizers
    ITKStatistics
)

#Set Headers:
set (${PROJECT_NAME}_HEADERS
    rpiTRex.hxx
    rpiTRex.cxx
    )

# Create TRex library
ADD_LIBRARY(           libTRex rpiTRex.cxx )
TARGET_LINK_LIBRARIES( libTRex ${LIBRARIES} )
SET_TARGET_PROPERTIES( libTRex PROPERTIES OUTPUT_NAME "RPITRex" )


# Create Opitmus executable
ADD_EXECUTABLE(        exeTRex rpiTRexExecutable.cxx )
TARGET_LINK_LIBRARIES( exeTRex ${LIBRARIES})
SET_TARGET_PROPERTIES( exeTRex PROPERTIES OUTPUT_NAME "rpiTRex" )


# Install rules
INSTALL( TARGETS libTRex exeTRex
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/TRex)

