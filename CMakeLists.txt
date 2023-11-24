cmake_minimum_required(VERSION 3.22)
project(ft_nm C)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_C_STANDARD 23)

include_directories(./inc)
add_compile_options(-g -Wall -Werror -Wextra)

add_executable(ft_nm src/main.c src/debug_utils.c src/file_io.c src/main.c src/parsing.c
        src/symbols.c)