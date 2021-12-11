cmake_minimum_required(VERSION 3.10)

project(so_arnold_plugins)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

option(BUILD_SHADERS "Build shader plugins" ON)
option(BUILD_PROCEDURALS "Build procedurals" ON)

# We might as well do this first as 99% of this repo will need Arnold.
find_package(Arnold REQUIRED)

if (BUILD_SHADERS) 
	add_subdirectory(shaders)
endif()

if (BUILD_PROCEDURALS)
	add_subdirectory(procedurals)
endif()
