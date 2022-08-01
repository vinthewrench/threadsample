SET (CMAKE_C_COMPILER             "clang")
SET (CMAKE_CXX_COMPILER            "clang++")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O2 -ffast-math -ftree-vectorize ${EXTRA_FLAGS}")


cmake_minimum_required(VERSION 3.0)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

project(foo)

find_package(Threads)
 
set(SOURCES
	src/main.cpp
	src/FooMgr.cpp
)

 
add_executable(foo ${SOURCES})

set_target_properties(foo PROPERTIES
				CXX_STANDARD 17
				CXX_EXTENSIONS OFF
				)
 
set_target_properties(foo PROPERTIES
			OUTPUT_NAME "foo"
			)

			  
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(

    ${EXTRA_INCLUDES} )

link_directories(
    ${EXTRA_INCLUDES}
    )

target_link_libraries(foo
	 PRIVATE
	 ${CMAKE_THREAD_LIBS_INIT}
 	 Threads::Threads
 	)

target_include_directories(foo
	PRIVATE
	src
)

set(CMAKE_BINARY_DIR "bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_custom_target(copy_assets
     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Assets ${CMAKE_BINARY_DIR}
 )

# add this in if you want to copy an assets directory too
#add_dependencies(foo copy_assets)
