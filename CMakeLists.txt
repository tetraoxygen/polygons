cmake_minimum_required(VERSION 3.16)

project(polygons)

add_executable(polygons src/spaceObjectTest.cpp)

install(TARGETS polygons)
set_property(TARGET polygons PROPERTY CXX_STANDARD 14)


enable_testing()
add_test(NAME polygons COMMAND polygons)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(polygons -static)
ENDIF()