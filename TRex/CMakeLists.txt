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
IF( NOT TCLAP_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires TCLAP and TCLAP was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE_DIRECTORIES( ${TCLAP_INCLUDE_DIR} )


# Add MIPS include directory
INCLUDE_DIRECTORIES( ${MIPS_IMAGEPKG_INCLUDE_DIRS} )


# Set used libraries
SET(LIBRARIES
    ITKIO
    ITKNumerics
    ITKStatistics
)


# Create TRex library
ADD_LIBRARY(           libTRex rpiTRex.cxx )
TARGET_LINK_LIBRARIES( libTRex ${LIBRARIES} )
SET_TARGET_PROPERTIES( libTRex PROPERTIES OUTPUT_NAME "RPITRex" )


# Create Opitmus executable
ADD_EXECUTABLE(        exeTRex rpiTRexExecutable.cxx )
TARGET_LINK_LIBRARIES( exeTRex libTRex mipsItkImageIO )
SET_TARGET_PROPERTIES( exeTRex PROPERTIES OUTPUT_NAME "rpiTRex" )


# Install rules
INSTALL( TARGETS libTRex exeTRex
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static)
