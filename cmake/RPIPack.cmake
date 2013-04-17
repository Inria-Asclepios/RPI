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

include (InstallRequiredSystemLibraries)

## #################################################################
## Global settings
## #################################################################
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  execute_process(COMMAND uname -r
    COMMAND sed "s/\\.PAE//" # Getting of any PAE keyword
    COMMAND sed "s/.*\\.\\(\\w*\\.\\w*\\)$/\\1/"
    OUTPUT_VARIABLE PACKAGE_EXTENSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.${PACKAGE_EXTENSION}")
else()
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}-src")

set(CPACK_PACKAGE_VENDOR "http://www-sop.inria.fr/asclepios/software/RPI/")
set(CPACK_PACKAGE_CONTACT "MedInria <medinria-userfeedback@inria.fr>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}: Registration Programming Interface. Wrapping API for registration algorithms")
set(CPACK_PACKAGE_DESCRIPTION "Registration Programming interface, is an API to help developer wrap their registration algorithms in a single fashion.")
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_BUILD})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)

set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})


include(CPack)
