//
// Created by loumouli on 11/10/23.
//

#include "ft_nm.h"

int32_t print_error(const char* filename, const char* error) {
  ft_putstr_fd("ft_nm: ", 2);
  ft_putstr_fd(filename, 2);
  ft_putstr_fd(": ", 2);
  ft_putstr_fd(error, 2);
  ft_putstr_fd("\n", 2);
  return 1;
}

inline char hex_char(int value) {
  if (value >= 0 && value < 10) {
    return '0' + value;
  }
  return 'a' + (value - 10);
}

// Function to convert an unsigned long to a hexadecimal string with leading zeros
inline void ulong_to_hex_str(unsigned long value, char* str, int len) {
  for (int i = len - 1; i >= 0; i--) {
    str[i] = hex_char(value % 16);
    value /= 16;
  }
  str[len] = '\0'; // Null-terminate the string
}

// Function to print an unsigned long in hexadecimal format
void print_hex(unsigned long value, int fd, const bool arch) {
  char hex_string[17]; // 16 characters for the hexadecimal representation + 1 for null terminator
  ft_memset(hex_string, '0', 17);
  if (arch)
    ulong_to_hex_str(value, hex_string, 16);
  else
    ulong_to_hex_str(value, hex_string, 8);
  ft_putstr_fd(hex_string, fd);
}

Elf64_Shdr* get_header_type_64(const t_nm* file, const uint64_t type) {
  for (const t_list* tmp = file->lst_shdr_64; tmp != NULL; tmp = tmp->next) {
    if (((Elf64_Shdr *)tmp->content)->sh_type == type)
      return tmp->content;
  }
  return NULL;
}

Elf64_Shdr* get_header_idx_64(const t_nm* file, const uint32_t shndx) {
  uint32_t idx = 0;
  for (const t_list* tmp = file->lst_shdr_64; tmp != NULL; tmp = tmp->next) {
    if (idx == shndx)
      return tmp->content;
    idx += 1;
  }
  return NULL;
}

Elf32_Shdr* get_header_type_32(const t_nm* file, const uint64_t type) {
  for (const t_list* tmp = file->lst_shdr_32; tmp != NULL; tmp = tmp->next) {
    if (((Elf32_Shdr *)tmp->content)->sh_type == type)
      return tmp->content;
  }
  return NULL;
}

Elf32_Shdr* get_header_idx_32(const t_nm* file, const uint32_t shndx) {
  uint32_t idx = 0;
  for (const t_list* tmp = file->lst_shdr_32; tmp != NULL; tmp = tmp->next) {
    if (idx == shndx)
      return tmp->content;
    idx += 1;
  }
  return NULL;
}

void free_sym(void* sym) {
  free(((t_sym *)sym)->name);
  free(sym);
}