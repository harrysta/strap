cmake_minimum_required(VERSION 3.14)
project(strap)

file(GLOB SOURCES src/*.c)
file(GLOB TESTS tests/*.c)

include_directories(include)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(test ${TESTS})
target_link_libraries(test PUBLIC ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES include/strap.h DESTINATION include)
