cmake_minimum_required( VERSION 2.8 )

set( PROJ_NAME "opt_cmp" )
set( PROJ_EXE "opt_cmp" )

file( GLOB_RECURSE PROJ_SOURCES "src/*.cc" )
file( GLOB_RECURSE PROJ_HACKS "hacks/*.cc" )
set( PROJ_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" )

project( ${PROJ_NAME} )

#set( Boost_NO_BOOST_CMAKE TRUE )
add_subdirectory( bayesopt )
set( BAYESOPT_INCLUDES "bayesopt/include" "bayesopt/utils" )

add_subdirectory( cpplogo )
set( CPPLOGO_INCLUDES "cpplogo/include" )

set( CMAKE_CXX_FLAGS "-std=c++14 -Wall -pthread -static-libstdc++" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set( CMAKE_CXX_FLAGS_DEBUG "-gdwarf-3" )

#TODO: Boost stuff? - for cpplogo
set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( Boost_USE_STATIC_RUNTIME OFF )
set( BOOST_ROOT "/scratch/merriler/local" )
set( BOOST_LIBRARYDIR "/scratch/merriler/local/lib" )
find_package( Boost 1.61.0 COMPONENTS log REQUIRED )
include_directories( SYSTEM ${Boost_INCLUDE_DIR} )

message( STATUS ${Boost_LIBRARIES} )

include_directories( ${PROJ_INCLUDES} )
include_directories( SYSTEM  ${BAYESOPT_INCLUDES} ${CPPLOGO_INCLUDES} )
add_executable( ${PROJ_EXE} ${PROJ_SOURCES} ${PROJ_HACKS} )
target_link_libraries( ${PROJ_EXE} bayesopt cpplogo ${Boost_LIBRARIES} )
