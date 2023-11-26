cmake_minimum_required(VERSION 3.22)
project(ft_nm C)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_C_STANDARD 99)

include_directories(./inc ./libft/inc)
add_compile_options(-gdwarf-4 -Wall -Werror -Wextra -O3 -mavx -msse2)

add_executable(ft_nm src/main.c src/file_io.c src/main.c src/parsing.c src/symbols.c src/sym_cmp.c src/filter_sym.c)
add_subdirectory(libft)
target_link_libraries(ft_nm ft)
