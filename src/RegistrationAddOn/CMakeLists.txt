
set(TARGET_NAME registrationFactory)

include(${ITK_USE_FILE})

set(${TARGET_NAME}_HEADERS    
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

# Create convertLinearToDF executable
ADD_EXECUTABLE(             ${TARGET_NAME} )
TARGET_LINK_LIBRARIES(      ${TARGET_NAME} PUBLIC ${LIBRARIES} ITKOptimizers ITKSmoothing)
TARGET_SOURCES(             ${TARGET_NAME} PRIVATE RegistrationFactory.cxx ${GETOPT_SRC})                              
TARGET_SOURCES(             ${TARGET_NAME} PUBLIC 
                              FILE_SET "publicHeaders" 
                              TYPE HEADERS FILES ${${TARGET_NAME}_HEADERS}
                              BASE_DIRS ${CMAKE_SOURCE_DIR}/src/RegistrationAddOn)
TARGET_INCLUDE_DIRECTORIES( ${TARGET_NAME} INTERFACE
                              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/RegistrationAddOn>
                              $<INSTALL_INTERFACE:include/${TARGET_NAME}>)

SET (FILESET_INSTALL_DIR_publicHeadersCommon Common)
SET (FILESET_INSTALL_DIR_publicHeadersRegistrationAddOn RegistrationAddOn)

export_and_install_libs(${TARGET_NAME})             


