
add_subdirectory(Testing)

option(WithHdf5_Example "Include hdf5 example" OFF)

if (WithHdf5_Example)
  add_subdirectory(hdf5Testing)
endif()
