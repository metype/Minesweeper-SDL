cmake_minimum_required(VERSION 3.18)
project(Minesweeper)

option(DEBUG "Compiles with debugging symbols.")

set(CMAKE_CXX_STANDARD 20)

if(DEBUG)
	set(COMPILER_FLAGS -g -rdynamic)
else()
	# -Ofast enables a good handful of flags, some of these may cause issues
	# So if you're getting hard-to-diagnose and downright strange bugs?
	# maybe try disabling some of those flags 
	# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-Ofast

	# For this project it's strictly very unneccesary to be honest
	set(COMPILER_FLAGS -Ofast -Wall -c -s -Wfatal-errors)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/CMakeModules")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

file(GLOB SOURCES sources/*.cpp)
file(GLOB HEADERS headers/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

find_package(SDL2 REQUIRED)

if(TARGET SDL2::SDL2main)
	# It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
	target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)

find_package(SDL2TTF)
target_link_libraries(${PROJECT_NAME} ${SDL2TTF_LIBRARY})

include_directories(${SDL2TTF_INCLUDE_DIR})


target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILER_FLAGS})