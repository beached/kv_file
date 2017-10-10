cmake_minimum_required( VERSION 2.8.12 )

project( kv_file_prj )

find_package( Boost 1.58.0 COMPONENTS unit_test_framework REQUIRED )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options(-std=c++14 -Weverything -pthread -Wno-c++98-compat -g -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -g -Wall -pthread -Wno-deprecated-declarations)
	endif( )
endif( )

include( ExternalProject )

externalproject_add(
    header_libraries_prj
    GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

enable_testing( )
add_definitions( -DBOOST_TEST_DYN_LINK ) 

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
add_dependencies( kv_file header_libraries_prj )

add_executable( kv_file_test_bin ${HEADER_FILES} ${TEST_FOLDER}/kv_file_test.cpp )
target_link_libraries( kv_file_test_bin kv_file ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} )
add_test( kv_file_test, kv_file_test_bin )

install( TARGETS kv_file DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw )
