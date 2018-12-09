cmake_minimum_required( VERSION 2.8.12 )

project( kv_file_prj )

enable_testing( )

find_package( Boost 1.58.0 COMPONENTS system iostreams filesystem regex unit_test_framework REQUIRED )

include( "${CMAKE_SOURCE_DIR}/dependent_projects/CMakeListsCompiler.txt" )
add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	#	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	set( SHOULD_BUILD_DEPS ON )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )

	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub header_libraries_prj date_prj )
	include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
endif( )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

include_directories( "./include" )

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
target_link_libraries( kv_file_test_bin kv_file ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} )
add_test( kv_file_test, kv_file_test_bin )

install( TARGETS kv_file DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw )

