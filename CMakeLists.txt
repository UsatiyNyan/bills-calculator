cmake_minimum_required(VERSION 3.15)
project(bills-calculator
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>
        -Wshadow
        -Wfloat-equal
        -Wcast-qual
        -Wlogical-op
        -Werror
        -Wcast-align
)

add_subdirectory(modules)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_frontend)
