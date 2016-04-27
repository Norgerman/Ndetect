cmake_minimum_required(VERSION 2.8)

project(Ndetect)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT "./3rd/boost")

find_package(Boost 1.60.0 REQUIRED COMPONENTS filesystem system)
find_package(OpenCV REQUIRED)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (UNIX) 
    set(CMAKE_CXX_COMPILER_ENV_VAR "g++")
elseif (APPLE)
    set(CMAKE_CXX_COMPILER_ENV_VAR "clang++")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D CASECADE_NAME_1=\\\"${PROJECT_SOURCE_DIR}/data/haarcascade_frontalface_alt.xml\\\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D CASECADE_NAME_2=\\\"${PROJECT_SOURCE_DIR}/data/haarcascade_frontalface_alt2.xml\\\"")
	set(CMAKE_CXX_FLAGS_RELEASE "/Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS_RELEASE "-Weverything")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra")
endif()

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D CASECADE_NAME_1=\\\"${PROJECT_SOURCE_DIR}/data/haarcascade_frontalface_alt.xml\\\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D CASECADE_NAME_2=\\\"${PROJECT_SOURCE_DIR}/data/haarcascade_frontalface_alt2.xml\\\"")
	set(CMAKE_CXX_FLAGS_DEBUG   "-g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL  "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE  "-O4 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${PROJECT_SOURCE_DIR}/include/*.h*")

include_directories("${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(ndetect ${sources} ${headers})

target_link_libraries(ndetect ${OpenCV_LIBS} ${Boost_LIBRARIES})
