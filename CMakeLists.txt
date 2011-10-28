project(test_moost)

cmake_minimum_required(VERSION 2.6)
#set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_COLOR_MAKEFILE True)

if (UNIX)
	add_definitions("-std=c++0x -W -Wall")
endif (UNIX)

add_subdirectory(moost)
add_subdirectory(test)
