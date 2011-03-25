# Project name
PROJECT( DIFFEOMORPHIC_DEMONS )

# Define the minimum CMake version needed
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )



#======================================
#
# CMake for standard project
#
#======================================

# Check if ITK is found and include useful files
FIND_PACKAGE( ITK )
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )

# Set used libraries
SET(LIBRARIES
    ITKNumerics
    ITKStatistics
    #ITKIO
    #ITKAlgorithms
    #ITKCommon
    #InrimageNG
)

# Create old diffeomorphic demons executable
ADD_EXECUTABLE(        oldExeDiffeomorphicDemons DemonsRegistration.cxx )
TARGET_LINK_LIBRARIES( oldExeDiffeomorphicDemons ${LIBRARIES} )
SET_TARGET_PROPERTIES( oldExeDiffeomorphicDemons PROPERTIES OUTPUT_NAME "DemonsRegistration" )

# Install rules
INSTALL( TARGETS oldExeDiffeomorphicDemons RUNTIME DESTINATION bin )



#======================================
#
# CMake for RPI project
#
#======================================

# Check if TCLAP is found and include directory
IF( NOT TCLAP_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} (RPI) requires TCLAP and TCLAP was not found. ${PROJECT_NAME} (RPI) will not be built." )
    RETURN()
ENDIF()
INCLUDE_DIRECTORIES( ${TCLAP_INCLUDE_DIR} )

# Set used libraries
SET(LIBRARIES
    #ITKIO
    ITKNumerics
    ITKStatistics
)

# Create diffeomorphic demons library
ADD_LIBRARY(           libDiffeomorphicDemons rpiDiffeomorphicDemons.cxx )
TARGET_LINK_LIBRARIES( libDiffeomorphicDemons ${LIBRARIES} )
SET_TARGET_PROPERTIES( libDiffeomorphicDemons PROPERTIES OUTPUT_NAME "RPIDiffeomorphicDemons" )

# Create diffeomorphic demons executable
ADD_EXECUTABLE(        exeDiffeomorphicDemons rpiDiffeomorphicDemonsExecutable.cxx )
TARGET_LINK_LIBRARIES( exeDiffeomorphicDemons libDiffeomorphicDemons )
SET_TARGET_PROPERTIES( exeDiffeomorphicDemons PROPERTIES OUTPUT_NAME "rpiDiffeomorphicDemons" )

# Install rules
INSTALL( TARGETS libDiffeomorphicDemons exeDiffeomorphicDemons
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static)
