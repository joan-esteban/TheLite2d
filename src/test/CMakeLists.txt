cmake_minimum_required(VERSION 2.8.2)
IF (  "${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "test$"   )
	SET (BASE_TEST_SRC_DIR 		${CMAKE_CURRENT_SOURCE_DIR} )
ELSE ()
	SET (BASE_TEST_SRC_DIR 		${CMAKE_CURRENT_SOURCE_DIR}/src/test )
ENDIF ()
MESSAGE( "CMAKE_BINARY_DIR" ${CMAKE_BINARY_DIR} )
# Download and unpack googletest at configure time
configure_file(${BASE_TEST_SRC_DIR}/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories(BEFORE SYSTEM
    "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
else()
  target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
    "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
endif()
# /////////////////////////////////////////////////////////

SET (ENGINE_BASE_DIR 		${BASE_TEST_SRC_DIR}/../engine/)
SET (TEST_BASE_DIR 		${BASE_TEST_SRC_DIR})
file ( GLOB_RECURSE SRCS_TEST
  "${TEST_BASE_DIR}/src/*.cpp"
  "${TEST_BASE_DIR}/src/*.h"
)


SOURCE_GROUP( "test" FILES ${SRCS_TEST} )
#SOURCE_GROUP( "external" FILES ${SRCS_ENGINE_TEST} )

include_directories(${THELITE2D_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SPDLOG_INCLUDE})
include_directories(${GAME_BASE_DIR})

add_definitions(-D__UNIT_TEST__)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test ${SRCS_TEST}  )

target_link_libraries(test gmock_main)

target_link_libraries(test ${LIBRARY_THELITE2D_NAME})

target_link_libraries(test ${SDL2_LIBRARY} ) 
target_link_libraries(test ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})


add_test(NAME unittest COMMAND test)
