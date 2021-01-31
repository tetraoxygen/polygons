cmake_minimum_required(VERSION 3.16)

project(polygons)

if(LINUX)
	set(SFML_DIR "/usr/include/SFML/")
elseif(APPLE)
	set(SFML_DIR "/usr/local/include/lib/cmake/SFML")
endif()

find_package(SFML 2 COMPONENTS graphics REQUIRED)

add_executable(polygons src/game.cpp)

set_property(TARGET polygons PROPERTY CXX_STANDARD 14)

target_link_libraries(polygons sfml-graphics)

install(TARGETS polygons)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(polygons -static)
ENDIF()