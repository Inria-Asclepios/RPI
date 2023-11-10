
SET(TARGET_NAME common)


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

# Create RPI_INTERFACE interface
ADD_LIBRARY(                ${TARGET_NAME} INTERFACE)                        
TARGET_SOURCES(             ${TARGET_NAME} INTERFACE 
                              FILE_SET "publicHeadersCommon" 
                              TYPE HEADERS 
                              #BASE_DIRS .
                              FILES ${${TARGET_NAME}_HEADERS_COMMON})
TARGET_SOURCES(             ${TARGET_NAME} INTERFACE 
                              FILE_SET "publicHeadersRegistrationAddOn" 
                              TYPE HEADERS 
                              BASE_DIRS ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn
                              FILES ${${TARGET_NAME}_HEADERS_REGISTRATIONADDON})
TARGET_INCLUDE_DIRECTORIES( ${TARGET_NAME} INTERFACE
                              "$<BUILD_INTERFACE:src/common src/RegistrationAddOn>"
                              "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>include/common;$<INSTALL_PREFIX>include/RegistrationAddOn>")
                

SET (FILESET_INSTALL_DIR_publicHeadersCommon Common)
SET (FILESET_INSTALL_DIR_publicHeadersRegistrationAddOn RegistrationAddOn)

export_and_install_libs(${TARGET_NAME})
