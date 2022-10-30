cmake_minimum_required(VERSION 3.14)
project(strap)

file(GLOB SOURCES src/*.c)
file(GLOB TESTS tests/*.c)

option(STRAP_BUILD_TESTS "Include unit test binary" OFF)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
include_directories(include)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

if (STRAP_BUILD_TESTS)
	add_executable(test ${TESTS})
	target_link_libraries(test PUBLIC ${PROJECT_NAME})
endif ()

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES include/strap.h DESTINATION include)
