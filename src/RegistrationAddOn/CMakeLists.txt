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

SET(TARGET_NAME registrationFactory)

include(${ITK_USE_FILE})

SET(${TARGET_NAME}_HEADERS    
    itkAffineRegistrationMethod.h
    itkAffineRegistrationMethod.txx
    itkCenterImageRegistrationMethod.h

    itkFixedPointInverseDisplacementFieldImageFilter.h
    itkFixedPointInverseDisplacementFieldImageFilter.txx

    itkGeneralTransform.h
    itkGeneralTransform.txx
    itkImageRegistrationFactory.h
    itkImageRegistrationFactory.txx
    itkLandmarkRegistrationMethod.h
    itkLandmarkRegistrationMethod.txx
    itkNullRegistrationMethod.h
    itkRegistrationFactory.h
    itkRegistrationFactory.txx
    itkRegistrationMethod.h
    itkRegistrationMethod.txx
    itkRigidRegistrationMethod.h
    itkRigidRegistrationMethod.txx

    itkStationaryVelocityFieldExponential.h
    itkStationaryVelocityFieldExponential.txx
    itkStationaryVelocityFieldTransform.h
    itkStationaryVelocityFieldTransform.txx
    itkTransformToVelocityFieldSource.h
    itkTransformToVelocityFieldSource.txx

    rpiDisplacementFieldTransform.h
    rpiDisplacementFieldTransform.txx
    )

IF(WIN32 AND NOT CYGWIN AND NOT MINGW)
  LIST(APPEND GETOPT_SRC
    getoptcompat/getopt.c
    getoptcompat/getopt_long.c
  )
ENDIF()


set(LIBRARIES
    ${ITKIO_LIBRARIES}
    ${ITKIOPhilipsREC_LIBRARIES}
    ${ITK_TRANSFORM_LIBRARIES}
)


# Create RegistrationAddOn executable
ADD_EXECUTABLE(             ${TARGET_NAME} )
TARGET_LINK_LIBRARIES(      ${TARGET_NAME} PRIVATE ${LIBRARIES} ITKOptimizers ITKSmoothing)
TARGET_SOURCES(             ${TARGET_NAME} PRIVATE RegistrationFactory.cxx ${GETOPT_SRC})                              
TARGET_SOURCES(             ${TARGET_NAME} PUBLIC 
                              FILE_SET "publicHeaders" 
                              TYPE HEADERS FILES ${${TARGET_NAME}_HEADERS}
                              BASE_DIRS ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn)
TARGET_INCLUDE_DIRECTORIES( ${TARGET_NAME} INTERFACE
                              "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/RegistrationAddOn>"
                              "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>include/${TARGET_NAME}>")

SET (FILESET_INSTALL_DIR_publicHeadersCommon common)
SET (FILESET_INSTALL_DIR_publicHeadersRegistrationAddOn RegistrationAddOn)
                              
export_and_install_libs(${TARGET_NAME})
