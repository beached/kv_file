cmake_minimum_required( VERSION 2.8.12 )

project( dependency_prj )

find_package( Boost 1.60.0 REQUIRED )

include( "CMakeListsCompiler.txt" )

include_directories( "./include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

include( "dependencies.txt" )

add_library( dependency_stub STATIC stub.cpp )
add_dependencies( dependency_stub header_libraries_prj date_prj )
