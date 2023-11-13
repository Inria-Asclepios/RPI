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

SET(TARGET_NAME fuseTransformations)

find_package(TinyXML2 REQUIRED)

include(${ITK_USE_FILE})

SET(${TARGET_NAME}_HEADERS_REGISTRATIONADDON   
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkAffineRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkAffineRegistrationMethod.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkCenterImageRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkFixedPointInverseDisplacementFieldImageFilter.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkFixedPointInverseDisplacementFieldImageFilter.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkGeneralTransform.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkGeneralTransform.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkImageRegistrationFactory.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkImageRegistrationFactory.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkLandmarkRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkLandmarkRegistrationMethod.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkNullRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRegistrationFactory.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRegistrationFactory.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRegistrationMethod.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRigidRegistrationMethod.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkRigidRegistrationMethod.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkStationaryVelocityFieldExponential.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkStationaryVelocityFieldExponential.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkStationaryVelocityFieldTransform.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkStationaryVelocityFieldTransform.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkTransformToVelocityFieldSource.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/itkTransformToVelocityFieldSource.txx
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/rpiDisplacementFieldTransform.h
    ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn/rpiDisplacementFieldTransform.txx
    )
    
SET(${TARGET_NAME}_HEADERS_COMMON  
    ${CMAKE_SOURCE_DIR}/src/common/rpiCommonTools.hxx
    ${CMAKE_SOURCE_DIR}/src/common/rpiCommonTools.txx
    ${CMAKE_SOURCE_DIR}/src/common/rpiObserver.txx
    ${CMAKE_SOURCE_DIR}/src/common/rpiObserver.hxx
    ${CMAKE_SOURCE_DIR}/src/common/rpiRegistrationMethod.hxx
    ${CMAKE_SOURCE_DIR}/src/common/rpiRegistrationMethod.txx
    ${CMAKE_SOURCE_DIR}/src/common/vnl_sd_matrix_tools.h
    ${CMAKE_SOURCE_DIR}/src/common/vnl_sd_matrix_tools.txx
    )

# Set used libraries
SET(LIBRARIES
    ${ITKIO_LIBRARIES}
    ${ITKIOPhilipsREC_LIBRARIES}
    ${ITK_TRANSFORM_LIBRARIES}
	${TinyXML2_LIBRARY}
)


# Create convertLinearToDF executable
ADD_EXECUTABLE(             ${TARGET_NAME} )
TARGET_LINK_LIBRARIES(      ${TARGET_NAME} PRIVATE ${LIBRARIES} )
TARGET_SOURCES(             ${TARGET_NAME} PRIVATE rpiFuseTransformations.cxx)
TARGET_SOURCES(             ${TARGET_NAME} PUBLIC 
                              FILE_SET "publicHeadersCommon" 
                              TYPE HEADERS 
                              BASE_DIRS ${CMAKE_SOURCE_DIR}/src/common
                              FILES ${${TARGET_NAME}_HEADERS_COMMON})
TARGET_SOURCES(             ${TARGET_NAME} PUBLIC 
                              FILE_SET "publicHeadersRegistrationAddOn" 
                              TYPE HEADERS 
                              BASE_DIRS ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn
                              FILES ${${TARGET_NAME}_HEADERS_REGISTRATIONADDON})                         
TARGET_INCLUDE_DIRECTORIES( ${TARGET_NAME} PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/common;${CMAKE_SOURCE_DIR}/src/RegistrationAddOn;${TinyXML2_INCLUDE_DIR}>"
                              "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>include/common;$<INSTALL_PREFIX>include/RegistrationAddOn>")

SET (FILESET_INSTALL_DIR_publicHeadersCommon common)
SET (FILESET_INSTALL_DIR_publicHeadersRegistrationAddOn RegistrationAddOn)
                              
export_and_install_libs(${TARGET_NAME})
