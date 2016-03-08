cmake_minimum_required(VERSION 2.8)

project(Ndetect)

find_package(OpenCV REQUIRED)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

if (UNIX) 
    SET (CMAKE_CXX_COMPILER_ENV_VAR "g++")
elseif (APPLE)
    SET (CMAKE_CXX_COMPILER_ENV_VAR "clang++")
endif()

if (NOT MSVC)
	SET (CMAKE_CXX_FLAGS "-std=c++11")
	SET (CMAKE_CXX_FLAGS_DEBUG   "-g")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL  "-Os -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELEASE  "-O4 -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif()

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${PROJECT_SOURCE_DIR}/include/*.h*")

include_directories("${PROJECT_SOURCE_DIR}/include")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(ndetect ${sources} ${headers})

target_link_libraries(ndetect ${OpenCV_LIBS})

if (CMAKE_BUILD_TYPE MATCHES "Release")
	if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set_target_properties(ndetect PROPERTIES COMPILE_FLAGS "/Wall")
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_target_properties(ndetect PROPERTIES COMPILE_FLAGS "-Weverything")
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		set_target_properties(ndetect PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
	endif()
endif()

set_target_properties(ndetect PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${PROJECT_SOURCE_DIR}/include/stdafx.h")

cotire(ndetect)
