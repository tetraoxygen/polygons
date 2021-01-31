cmake_minimum_required(VERSION 3.16)

project(polygons)

set(SFML_DIR "/usr/local/include/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(polygons src/game.cpp)

target_link_libraries(polygons sfml-graphics sfml-audio)

install(TARGETS polygons)
set_property(TARGET polygons PROPERTY CXX_STANDARD 14)


enable_testing()
add_test(NAME polygons COMMAND polygons)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(polygons -static)
ENDIF()