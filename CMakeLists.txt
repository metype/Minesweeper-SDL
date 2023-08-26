cmake_minimum_required(VERSION 3.18)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 20)
set(COMPILER_FLAGS -Ofast -fno-fast-math -Wall -c -s -fno-rtti -Wno-sign-compare -Wfatal-errors)
set(CMAKE_MODULE_PATH CMakeModules)

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