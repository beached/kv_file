cmake_minimum_required( VERSION 2.8.12 )

project( kv_file_prj )

find_package( Boost 1.60.0 REQUIRED )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")
if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options( -Weverything -pthread -Wno-c++98-compat -g -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options( -g -Wall -pthread -Wno-deprecated-declarations)
	endif( )
endif( )

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )
endif( )

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )

enable_testing( )

include_directories( "./include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

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
add_test( kv_file_test, kv_file_test_bin )

install( TARGETS kv_file DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw )

