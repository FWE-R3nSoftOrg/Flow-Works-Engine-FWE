cmake_minimum_required(VERSION 3.20)

project(FWE
    VERSION 0.1.0
    DESCRIPTION "Flow Works Engine — All-in-one engineering design and simulation platform"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Options
option(FWE_BUILD_TESTS      "Build test suite"           ON)
option(FWE_BUILD_TOOLS      "Build developer tools"      ON)
option(FWE_ENABLE_CUDA      "Enable NVIDIA CUDA compute" OFF)
option(FWE_ENABLE_ROCM      "Enable AMD ROCm compute"    OFF)
option(FWE_ENABLE_OPENCL    "Enable OpenCL fallback"     ON)

# Output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies
find_package(OpenCASCADE REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Optional GPU backends
if(FWE_ENABLE_CUDA)
    find_package(CUDA REQUIRED)
    add_compile_definitions(FWE_CUDA_ENABLED)
endif()

if(FWE_ENABLE_OPENCL)
    find_package(OpenCL)
    if(OpenCL_FOUND)
        add_compile_definitions(FWE_OPENCL_ENABLED)
    endif()
endif()

# Subdirectories
add_subdirectory(src/core)
add_subdirectory(src/model)
add_subdirectory(src/material)
add_subdirectory(src/environment)
add_subdirectory(src/simulate)
add_subdirectory(src/nodes)
add_subdirectory(src/formats)
add_subdirectory(src/ui)

if(FWE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(FWE_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Main executable
add_executable(fwe src/main.cpp)
target_link_libraries(fwe
    PRIVATE
        fwe_core
        fwe_ui
        Qt6::Core
        Qt6::Widgets
)

target_include_directories(fwe PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
