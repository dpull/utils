cmake_minimum_required(VERSION 3.5)
set(CMAKE_SUPPRESS_REGENERATION TRUE CACHE BOOL "Disable Zero Check Project")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(PROJ_NAME "c_utils_tests")
project(${PROJ_NAME})

if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -fsanitize=address")
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "")
add_subdirectory(googletest)

file(GLOB SOURCE_FILES 
    *.cpp
    *.cc
    *.c
    *.h
)

file(GLOB LIB_FILES 
    ../*.c
    ../*.h
)

source_group("tests" FILES ${SOURCE_FILES})
source_group("c_utils" FILES ${LIB_FILES})

include_directories(../)
include_directories(googletest/googletest/include)

add_executable(${PROJ_NAME} ${SOURCE_FILES} ${LIB_FILES})
target_link_libraries(${PROJ_NAME} gtest)

set_property(TARGET gmock PROPERTY FOLDER "googletest")
set_property(TARGET gmock_main PROPERTY FOLDER "googletest")
set_property(TARGET gtest PROPERTY FOLDER "googletest")
set_property(TARGET gtest_main PROPERTY FOLDER "googletest")

if(NOT MSVC)
    target_link_libraries(${PROJ_NAME} pthread)
endif()

message(STATUS *******************************************************)
message(STATUS CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME})
message(STATUS CMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION})
message(STATUS CMAKE_HOST_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME})
message(STATUS CMAKE_GENERATOR=${CMAKE_GENERATOR})
message(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
message(STATUS CMAKE_C_COMPILER=${CMAKE_C_COMPILER})
message(STATUS CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
message(STATUS CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})
message(STATUS CMAKE_CXX_STANDARD_LIBRARIES=${CMAKE_CXX_STANDARD_LIBRARIES})
message(STATUS CMAKE_C_FLAGS=${CMAKE_C_FLAGS})
message(STATUS CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})
message(STATUS CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS})
message(STATUS CMAKE_MODULE_LINKER_FLAGS=${CMAKE_MODULE_LINKER_FLAGS})
message(STATUS CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS})
message(STATUS CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR})
message(STATUS *******************************************************)
