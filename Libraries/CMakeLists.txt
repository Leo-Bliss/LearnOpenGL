cmake_minimum_required(VERSION 3.2)	
project(Libraries)

LIST(APPEND LibrariesSubDir "glad")
LIST(APPEND LibrariesSubDir "glfw")
LIST(APPEND LibrariesSubDir "glm")

set(PROJECT_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
foreach(dirname ${LibrariesSubDir}) 
	add_subdirectory(${dirname})
endforeach()