cmake_minimum_required(VERSION 3.10)

project(swaml-bitquant VERSION 1.0 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(SWAML_BITQUANT_ENABLE_TESTS "Enable tests for swaml bit quantization tool" OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

if (SWAML_BITQUANT_ENABLE_TESTS)
    add_subdirectory(tests)
endif ()

add_executable(main main.cpp)

target_link_libraries(main swaml_bitquant_lib)
