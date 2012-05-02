
set(targetName streamingDemo)

# Dependencies
if ( NOT VTK_USE_QVTK )
  message(SEND_ERROR "VTK was not configured with QVTK turned on.")
endif()

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

#Determine files to MOC as those containing Q_OBJECT.
foreach(fileIt ${HEADERS})
    file(STRINGS ${fileIt} foundString LIMIT_COUNT 1 REGEX "^[ \\t]*Q_OBJECT([^0-9A-Za-z_]|$)")
    string(LENGTH "${foundString}" foundLength)
    if( ${foundLength} GREATER 0 )
        set(HEADERS_MOC ${HEADERS_MOC} ${fileIt})
    endif( ${foundLength} GREATER 0 )
endforeach(fileIt)

qt4_wrap_cpp(SOURCES_MOC ${HEADERS_MOC})

add_executable(${targetName}
  ${SOURCES}
  ${SOURCES_MOC}
  ${HEADERS}
  ${HEADERS_MOC}
)

target_link_libraries(${targetName}
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  ${QT_LIBRARIES}
  QVTK
)

include_directories("../")

source_group("Generated Files" FILES ${SOURCES_MOC})
