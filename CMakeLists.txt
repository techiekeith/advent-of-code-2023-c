cmake_minimum_required(VERSION 3.16)
project(advent_of_code_2023_c C)

set(CMAKE_C_STANDARD 11)

include_directories(src include)

file(GLOB sources
        "include/*.h"
        "include/**/*.h"
        "src/*.c"
        "src/**/*.c"
)

add_executable(advent_of_code_2023_c ${sources})
target_link_libraries(advent_of_code_2023_c m)
