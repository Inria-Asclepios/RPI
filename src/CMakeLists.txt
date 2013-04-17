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

PROJECT( RPI )

# Define the minimum CMake version needed
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(${PROJECT_NAME}_HEADERS
    rpiCommonTools.hxx    
    rpiCommonTools.cxx    
    itkDisplacementFieldTransform.h
    itkDisplacementFieldTransform.txx
    itkStationaryVelocityFieldTransform.h
    rpiRegistrationMethod.hxx    
    rpiRegistrationMethod.cxx
    itkStationaryVelocityFieldTransform.txx
    itkStationaryVelocityFieldTransform.txx
    itkTransformToVelocityFieldSource.h
    itkTransformToVelocityFieldSource.txx
    itkGeneralTransform.h
    itkGeneralTransform.txx
    )

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include)


# Check if ITK is found and include useful files
FIND_PACKAGE( ITK )
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK which was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )

FIND_PACKAGE( TCLAP )
# Check if TCLAP is found and include directory
IF( NOT TCLAP_FOUND )
    MESSAGE( "The RPI utilities require TCLAP which was not found. They will not be built." )
    RETURN()
ENDIF()
INCLUDE_DIRECTORIES( ${TCLAP_INCLUDE_DIR} )

# Set used libraries
SET(LIBRARIES
    ITKIO
    ITKAlgorithms
    ITKCommon
    ITKBasicFilters
    ITKStatistics
    ITKNumerics
)

IF (PACKAGENAME EQUAL MIPS_REGISTRATIONPKG)
	# Add MIPS include directory
	INCLUDE_DIRECTORIES( ${MIPS_IMAGEPKG_INCLUDE_DIRS} )
	# Set used libraries
	SET(LIBRARIES ${LIBRARIES} mipsItkImageIO)
	ADD_DEFINITIONS(-DMIPS_FOUND)
ENDIF()

# Create ConvertLinearToDF executable
ADD_EXECUTABLE(        exeConvertLinearToDF rpiConvertLinearToDF.cxx )
TARGET_LINK_LIBRARIES( exeConvertLinearToDF ${LIBRARIES} )
SET_TARGET_PROPERTIES( exeConvertLinearToDF PROPERTIES OUTPUT_NAME "rpiConvertLinearToDF" )

# Create ConvertLinearToSVF executable
ADD_EXECUTABLE(        exeConvertLinearToSVF rpiConvertLinearToSVF.cxx )
TARGET_LINK_LIBRARIES( exeConvertLinearToSVF ${LIBRARIES} )
SET_TARGET_PROPERTIES( exeConvertLinearToSVF PROPERTIES OUTPUT_NAME "rpiConvertLinearToSVF" )

# Create rpiResampleImage executable
ADD_EXECUTABLE(        exeResampleImage rpiResampleImage.cxx )
TARGET_LINK_LIBRARIES( exeResampleImage ${LIBRARIES} )
SET_TARGET_PROPERTIES( exeResampleImage PROPERTIES OUTPUT_NAME "rpiResampleImage" )

# Check if TinyXML is found and include directory
IF( NOT TinyXML_FOUND AND NOT TARGET TinyXML )
    MESSAGE( "The rpiFuseTransformations utility requires TinyXML and TinyXML was not found. It will not be built." )
    RETURN()
ENDIF()

INCLUDE_DIRECTORIES( ${TinyXML_INCLUDE_DIR} )
# Create rpiFuseTransformations executable
ADD_EXECUTABLE(        exeFuseTransformations rpiFuseTransformations.cxx )
TARGET_LINK_LIBRARIES( exeFuseTransformations ${LIBRARIES} TinyXML )
SET_TARGET_PROPERTIES( exeFuseTransformations PROPERTIES OUTPUT_NAME "rpiFuseTransformations" )

