cmake_minimum_required( VERSION 2.8.12 )

project( kv_file_prj )

find_package( Boost 1.60.0 REQUIRED )

include( "${CMAKE_SOURCE_DIR}/dependent_projects/CMakeListsCompiler.txt" )

set( DEP_ROOT_DIR_BASE "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	set( DEP_ROOT_DIR "${DEP_ROOT_DIR_BASE}/debug" )
elseif( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
	set( DEP_ROOT_DIR "${DEP_ROOT_DIR_BASE}/release" )
endif( )

message( "Building for: ${CMAKE_BUILD_TYPE}" )
if( EXISTS "${DEP_ROOT_DIR}" )
	message( "${DEP_ROOT_DIR}" )
	message( "using dependency folder" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${DEP_ROOT_DIR}/install/include" )
	link_directories( "${DEP_ROOT_DIR}/install/lib" )
else( )
	message( "downloading and building deps" )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )
	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub ${DEP_PROJECT_DEPS} )
endif( )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

enable_testing( )

include_directories( "./include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

set( HEADER_FILES
	${HEADER_FOLDER}/kv_file.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/kv_file.cpp
)

add_library( kv_file STATIC ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( kv_file dependency_stub )

add_executable( kv_file_test_bin ${HEADER_FILES} ${TEST_FOLDER}/kv_file_test.cpp )
target_link_libraries( kv_file_test_bin kv_file )
add_test( kv_file_test kv_file_test_bin )

install( TARGETS kv_file DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw )

