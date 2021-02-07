cmake_minimum_required(VERSION 3.16)

project(polygons)

find_package(SFML 2 COMPONENTS graphics REQUIRED)

add_executable(polygons src/game.cpp)

set_property(TARGET polygons PROPERTY CXX_STANDARD 14)

target_link_libraries(polygons sfml-graphics)

install(TARGETS polygons)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(polygons -static)
ENDIF()