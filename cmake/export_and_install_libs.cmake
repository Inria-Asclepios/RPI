###############################################################################
# RPI
# Authors: F.Leray
# Created: 07/11/2023 
#
# Distributed under the BSD licence:
# Copyright (c) 2023, INRIA
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

INCLUDE(CMakePackageConfigHelpers)

MACRO(export_and_install_libs TARGET_NAME)

get_property(GENERATOR_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

GET_TARGET_PROPERTY(_SET_LIST_HEADERS ${TARGET_NAME} HEADER_SETS)
GET_TARGET_PROPERTY(_SET_LIST_INTERFACE_HEADERS ${TARGET_NAME} INTERFACE_HEADER_SETS)
LIST(APPEND _SET_LIST ${_SET_LIST_HEADERS} ${_SET_LIST_INTERFACE_HEADERS})
LIST(REMOVE_DUPLICATES _SET_LIST)

foreach(_SETS ${_SET_LIST})
    LIST(APPEND _TMP_FILESET_INST_CMD 
                "FILE_SET"
                "${_SETS}"
                "DESTINATION"
                "include/${FILESET_INSTALL_DIR_${_SETS}}")   
    LIST(APPEND _FILESET_INST_CMD ${_TMP_FILESET_INST_CMD})
    UNSET(_TMP_FILESET_INST_CMD)
endforeach()

if(${GENERATOR_MULTI_CONFIG})
  set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/bin)
  set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/bin)
  set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_BINARY_DIR}/bin)
  set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)

  set_target_properties( ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/lib)

  set_target_properties( ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/lib)
else()
  set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/bin)
  set_target_properties( ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/lib)
  set_target_properties( ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/lib)
endif()

    
INSTALL(
    TARGETS ${TARGET_NAME}
    EXPORT  ${TARGET_NAME}Targets

    RUNTIME   DESTINATION bin
    LIBRARY   DESTINATION lib
    ARCHIVE   DESTINATION lib
    FRAMEWORK DESTINATION lib
    INCLUDES  DESTINATION include
    RESOURCE  DESTINATION resources/${TARGET_NAME}
    PUBLIC_HEADER DESTINATION "include/${TARGET_NAME}"
    ${_FILESET_INST_CMD}
    )



write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/lib/cmake/${TARGET_NAME}/${TARGET_NAME}ConfigVersion.cmake"
  VERSION ${medInria_VERSION}
  COMPATIBILITY AnyNewerVersion
)

EXPORT(EXPORT ${TARGET_NAME}Targets
  FILE "${CMAKE_BINARY_DIR}/lib/cmake/${TARGET_NAME}/${TARGET_NAME}Targets.cmake"
  NAMESPACE ${PROJECT_NAME}::
)

SET(PACKAGE_INIT "@PACKAGE_INIT@")
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in @ONLY)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_BINARY_DIR}/lib/cmake/${TARGET_NAME}/${TARGET_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${TARGET_NAME}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)


SET(ConfigPackageLocation lib/cmake/${TARGET_NAME})
INSTALL(
  EXPORT ${TARGET_NAME}Targets
  FILE ${TARGET_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${ConfigPackageLocation}
)


INSTALL(FILES
  ${CMAKE_BINARY_DIR}/lib/cmake/${TARGET_NAME}/${TARGET_NAME}Config.cmake
  ${CMAKE_BINARY_DIR}/lib/cmake/${TARGET_NAME}/${TARGET_NAME}ConfigVersion.cmake
  DESTINATION lib/cmake/${TARGET_NAME}
  )

endmacro()
