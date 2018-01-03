project(RegistrationFactory)

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

#set ( RegistrationAddOnSRCS
#  itkRegistrationFactory-explicit-template.cxx
#)

set(${PROJECT_NAME}_LIBRARIES
  ITKIOImageBase
  ITKIOBMP
  ITKIOBioRad
  ITKIOBruker
  ITKIOHDF5
  ITKIOGDCM
  ITKIOGE
  ITKIOGIPL
  ITKIOJPEG
  ITKIOLSM
  ITKIOMeta
  ITKIOMINC
  ITKIONIFTI
  ITKIONRRD
  ${ITKIOPhilipsREC_LIBRARIES}
  ITKIOPNG
  ITKIOStimulate
  ITKIOVTK
  ITKIOTransformBase
  ITKIOTransformInsightLegacy
  ITKIOTransformHDF5
  ITKIOTransformMatlab
  ITKIOMRC
  ITKIOTIFF
)

add_executable(${PROJECT_NAME}
  RegistrationFactory.cxx
)

if(WIN32 AND NOT CYGWIN AND NOT MINGW)
 target_link_libraries(${PROJECT_NAME}
  ${${PROJECT_NAME}_LIBRARIES}
  ITKOptimizers
  getoptcompatlib
  )
else (WIN32 AND NOT CYGWIN AND NOT MINGW)
 target_link_libraries(${PROJECT_NAME}
   ${${PROJECT_NAME}_LIBRARIES}
   ITKOptimizers
   )
endif(WIN32 AND NOT CYGWIN AND NOT MINGW)

## #################################################################
## Install rules
## #################################################################

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
