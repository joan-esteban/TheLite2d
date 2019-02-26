CMAKE_MINIMUM_REQUIRED (VERSION 3.0.2)

IF (  "${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "examples$"   )
	SET (BASE_EXAMPLES_SRC_DIR 		${CMAKE_CURRENT_SOURCE_DIR} )
ELSE ()
	SET (BASE_EXAMPLES_SRC_DIR 		${CMAKE_CURRENT_SOURCE_DIR}/src/examples )
ENDIF ()

MESSAGE( "Examples path:" ${BASE_EXAMPLES_SRC_DIR} )

# Add each test as individual project
file ( GLOB_RECURSE SRCS_EXAMPLES
  "${BASE_EXAMPLES_SRC_DIR}/*.cpp" 
  "${BASE_EXAMPLES_SRC_DIR}/*.h" 
 
)

set(_src_root_path  "${BASE_EXAMPLES_SRC_DIR}")
foreach(_source IN ITEMS ${SRCS_EXAMPLES} )
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
	#set(_source_path_rel "${_source_path}" )
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	MESSAGE (" Group ${_group_path} ${_source}")
	include_directories(${THELITE2D_INCLUDE_DIR})
	include_directories(${SDL2_INCLUDE_DIR})
	include_directories(${SDL2_IMAGE_INCLUDE_DIR})
	include_directories(${SPDLOG_INCLUDE})
	include_directories(${GAME_BASE_DIR})
	add_executable(${_group_path} ${_source} )
	target_link_libraries(${_group_path} ${LIBRARY_THELITE2D_NAME} )
	target_link_libraries(${_group_path} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2MAIN_LIBRARY})
	#source_group("${_group_path}" FILES "${_source}")
endforeach()
	


