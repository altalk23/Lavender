cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Lavender VERSION 1.0.0)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})

    add_library(${PROJECT_NAME} SHARED
        src/main.cpp
        # Add any extra C++ source files here
    )

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wno-reorder-init-list
        )
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE
        include
        include/lavender
    )

    if (NOT DEFINED ENV{GEODE_SDK})
        message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
    else()
        message(STATUS "Found Geode: $ENV{GEODE_SDK}")
    endif()

    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

    setup_geode_mod(${PROJECT_NAME})
else()
    message(STATUS "Configuring Lavender as a subproject")
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE
        include
    )

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} INTERFACE
            -Wno-reorder-init-list
        )
    endif()
endif()