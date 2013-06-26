project(RegistrationAddOn)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(${PROJECT_NAME}_HEADERS
    itkAffineRegistrationMethod.h    
    itkAffineRegistrationMethod.txx
    itkCenterImageRegistrationMethod.h
    itkDisplacementFieldTransform.h
    itkDisplacementFieldTransform.txx
    itkFixedPointInverseDeformationFieldImageFilter.h
    itkFixedPointInverseDeformationFieldImageFilter.txx
    itkStationaryVelocityFieldExponential.h
    itkStationaryVelocityFieldExponential.txx
    itkStationaryVelocityFieldTransform.h
    itkStationaryVelocityFieldTransform.txx
    itkTransformToVelocityFieldSource.h
    itkTransformToVelocityFieldSource.txx
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
    itkTransformaToDeformationFieldFilter.h
    itkTransformaToDeformationFieldFilter.txx
    )

FIND_PACKAGE( ITK )
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${#} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )

IF(WIN32 AND NOT CYGWIN AND NOT MINGW)
  ADD_LIBRARY(getoptcompatlib STATIC
    getoptcompat/getopt.c
    getoptcompat/getopt_long.c
  )
ENDIF(WIN32 AND NOT CYGWIN AND NOT MINGW)

set ( RegistrationAddOnSRCS
  itkRegistrationFactory-explicit-template.cxx
)

add_library (${PROJECT_NAME}
  ${RegistrationAddOnSRCS} 
  )

target_link_libraries(RegistrationAddOn
    ${ITK_LIBRARIES}
 )

add_executable(RegistrationFactory
  RegistrationFactory.cxx
)

if(WIN32 AND NOT CYGWIN AND NOT MINGW)
 target_link_libraries(RegistrationFactory 
  RegistrationAddOn 
  getoptcompatlib
  )
else (WIN32 AND NOT CYGWIN AND NOT MINGW)
 target_link_libraries(RegistrationFactory
   RegistrationAddOn
   )
endif(WIN32 AND NOT CYGWIN AND NOT MINGW)

## #################################################################
## Install rules
## #################################################################

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
