cmake_minimum_required(VERSION 3.22)

project(CProjectTemplate VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(DEBUG "Enable Morse Code Debugging" OFF)

option(COMPILE_TEST "Enable Test Compilations" OFF)

option(HOST "Running on Host" OFF)

if(DEBUG)
    add_definitions(-DMORSE_DEBUG)
endif()

if(HOST)
    add_definitions(-DHOST)
endif()

set(TRANSLATELIB TRANSLIB)
set(EXECUTABLE_NAME Executable)

add_subdirectory(lib)
add_subdirectory(src)

if(COMPILE_TEST)

    set(TEST_EXECUTABLE test)

    add_subdirectory(unity)

    set_target_properties(${TEST_EXECUTABLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin")
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin")