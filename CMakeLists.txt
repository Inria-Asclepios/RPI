
cmake_minimum_required(VERSION 2.8.0)

project(streamingDemo)

FIND_PACKAGE(VTK REQUIRED)
include(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
include(${ITK_USE_FILE})

if (MSVC)
  # warning C4996: 'std::_Copy_impl': 
  # Function call with parameters that may be unsafe 
  # - this call relies on the caller to check that the passed values are correct.
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)
