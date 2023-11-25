//
// Created by louis on 11/2/23.
//

#include "ft_nm.h"

int base_filter(Elf64_Sym* sym_tab, const char* sym_str_tab) {
     if (sym_str_tab[sym_tab->st_name + ft_strlen(sym_str_tab + sym_tab->st_name) - 2] == '.' && (
          sym_str_tab[sym_tab->st_name + ft_strlen(sym_str_tab + sym_tab->st_name) - 1] == 'c' || sym_str_tab[
            sym_tab->st_name + ft_strlen(sym_str_tab + sym_tab->st_name) - 1] == 'o')) {
      return 1;
    }
    return 0;
}

int no_filter(Elf64_Sym* sym_tab, const char* sym_str_tab) {
    (void)sym_tab;
    (void)sym_str_tab;
    return 0;
}

int local_filter(Elf64_Sym* sym_tab, const char* sym_str_tab) {
    if (base_filter(sym_tab, sym_str_tab))
        return 1;
    if (ELF64_ST_BIND(sym_tab->st_info) == STB_LOCAL)
        return 1;
    return 0;
}

int defined_filter(Elf64_Sym* sym_tab, const char* sym_str_tab) {
    if (base_filter(sym_tab, sym_str_tab))
        return 1;
    if (sym_tab->st_shndx != SHN_UNDEF)
        return 1;
    return 0;
}

