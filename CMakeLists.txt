cmake_minimum_required( VERSION 2.8 )

set( PROJ_NAME "opt_cmp" )
set( PROJ_EXE "opt_cmp" )

file( GLOB_RECURSE PROJ_SOURCES "src/*.cc" )
set( PROJ_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" )

project( ${PROJ_NAME} )

add_subdirectory( clogo )
set( CLOGO_INCLUDES "${CMAKE_SOURCE_DIR}/clogo/include" )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_CXX_FLAGS "-std=c++14 -Wall" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set( CMAKE_CXX_FLAGS_DEBUG "-gdwarf-3" )

include_directories( ${PROJ_INCLUDES} ${CLOGO_INCLUDES} )
add_executable( ${PROJ_EXE} ${PROJ_SOURCES} )
target_link_libraries( ${PROJ_EXE} clogo )
