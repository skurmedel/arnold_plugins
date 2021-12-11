cmake_minimum_required(VERSION 3.10)

project(so_arnold_plugins)

option(BUILD_SHADERS "Build shader plugins" ON)
option(BUILD_PROCEDURALS "Build procedurals" ON)

if (BUILD_SHADERS) 
	add_subdirectory(shaders)
endif()

if (BUILD_PROCEDURALS)
	add_subdirectory(procedurals)
endif()
