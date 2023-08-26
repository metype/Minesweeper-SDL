cmake_minimum_required(VERSION 3.18)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 20)
set(COMPILER_FLAGS -Ofast -fno-fast-math -Wall -c -s -fno-rtti -Wno-sign-compare -Wfatal-errors)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILER_FLAGS})