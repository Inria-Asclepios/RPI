

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(targetName hdf5Testing)
add_executable(${targetName}
  ${SOURCES}
  ${HEADERS}
)

target_link_libraries(${targetName}
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  vtkhdf5
)

include_directories("../")
