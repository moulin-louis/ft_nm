//
// Created by louis on 11/2/23.
//
#include "ft_nm.h"

char* to_lower(char* str) {
  for (uint32_t i = 0; i < ft_strlen(str); i++) {
    str[i] = tolower(str[i]);
  }
  return str;
}

int sym_nocmp(Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j) {
  (void)sym_tab;
  (void)sym_str_tab;
  (void)i;
  (void)j;
  return 0;
}

int sym_strcmp(Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j) {
  uint32_t idx_start1 = 0;
  uint32_t idx_start2 = 0;
  while (sym_str_tab[sym_tab[i].st_name + idx_start1] == '_') {
    idx_start1 += 1;
  }
  while (sym_str_tab[sym_tab[j].st_name + idx_start2] == '_') {
    idx_start2 += 1;
  }
  char* str1 = to_lower(ft_strdup(sym_str_tab + sym_tab[i].st_name + idx_start1));
  char* str2 = to_lower(ft_strdup(sym_str_tab + sym_tab[j].st_name + idx_start2));
  int retval = strcmp(str1, str2);
  free(str1);
  free(str2);
  return retval;
}

int sym_rev_strcmp(Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j) {
  uint32_t idx_start1 = 0;
  uint32_t idx_start2 = 0;
  while (sym_str_tab[sym_tab[i].st_name + idx_start1] == '_') {
    idx_start1 += 1;
  }
  while (sym_str_tab[sym_tab[j].st_name + idx_start2] == '_') {
    idx_start2 += 1;
  }
  char* str1 = to_lower(ft_strdup(sym_str_tab + sym_tab[i].st_name + idx_start1));
  char* str2 = to_lower(ft_strdup(sym_str_tab + sym_tab[j].st_name + idx_start2));
  int retval = strcmp(str2, str1);
  return retval;
}