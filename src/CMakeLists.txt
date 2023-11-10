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


# Check if ITK is found and include useful files
FIND_PACKAGE( ITK )
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK which was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )


option(BUILD_ConvertLinearToDF   "Build command line ConvertLinearToDF executables"                       ON)
option(BUILD_ConvertLinearToSVF  "Build command line ConvertLinearToSVF executables"                      ON)
option(BUILD_FuseTransformations "Build command line FuseTransformations (need tinyXML2 lib) executables" OFF)
option(BUILD_ResampleImage       "Build command line ResampleImage executables"                           ON)
option(BUILD_RegistrationAddOn   "Build command line RegistrationAddOn executables"                       ON)
option(BUILD_examples            "Build command line examples"                                            OFF)

# TinyXML2
if (BUILD_FuseTransformations)
	find_package(TinyXML2 REQUIRED)
    IF( NOT ITK_FOUND )
        MESSAGE( "Project ${PROJECT_NAME} requires ITK which was not found. ${PROJECT_NAME} will not be built." )
        return()
    ENDIF()
endif()


#set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/common" "${CMAKE_SOURCE_DIR}/src/RegistrationAddOn" "${CMAKE_SOURCE_DIR}/src/examples")


add_subdirectory(common)
IF(${BUILD_ConvertLinearToDF})
    add_subdirectory(exeConvertLinearToDF)
ENDIF() 
IF(${BUILD_ConvertLinearToSVF})
    add_subdirectory(exeConvertLinearToSVF)
ENDIF()
IF(${BUILD_FuseTransformations})
    add_subdirectory(exeFuseTransformations)
ENDIF()
IF(${BUILD_ResampleImage})
    add_subdirectory(exeResampleImage)
ENDIF()
IF(${BUILD_RegistrationAddOn})
    add_subdirectory(RegistrationAddOn)
ENDIF() 
IF(${BUILD_examples})
    add_subdirectory(examples)
ENDIF()
