cmake_minimum_required(VERSION 3.22)
project(ft_nm C)

include_directories(./inc ./libft/inc)
add_compile_options(-Wall -Werror -Wextra -O3 -mavx -msse2 -march=native -fno-builtin)

add_executable(ft_nm src/main.c src/utils.c src/main.c src/parsing.c src/symbols.c src/sym_cmp.c src/sym_filter.c
        src/get_types.c)
add_subdirectory(libft)
target_link_libraries(ft_nm ft)
