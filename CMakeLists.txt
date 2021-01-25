cmake_minimum_required(VERSION 3.16)

set_property(TARGET tgt PROPERTY CXX_STANDARD 14)

project(polygons)

add_executable(polygons src/spaceObjectTest.cpp)

install(TARGETS polygons)

enable_testing()
add_test(NAME polygons COMMAND polygons)


IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(polygons -static)
ENDIF()