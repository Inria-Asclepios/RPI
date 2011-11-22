

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(targetName streamingDemo)
add_executable(${targetName}
  ${SOURCES}
  ${HEADERS}
)

target_link_libraries(${targetName}
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
)

include_directories("../")
