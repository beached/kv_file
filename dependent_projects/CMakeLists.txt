cmake_minimum_required( VERSION 2.8.12 )

project( dependency_prj )

find_package( Boost 1.58.0 COMPONENTS system iostreams filesystem regex unit_test_framework REQUIRED )

include( "CMakeListsCompiler.txt" )
add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

include_directories( "./include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

include( "dependencies.txt" )

add_library( dependency_stub STATIC stub.cpp )
add_dependencies( dependency_stub header_libraries_prj date_prj )
