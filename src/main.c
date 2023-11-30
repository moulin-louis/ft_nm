#include <stdio.h>

#include "ft_nm.h"

//check health elf header from file

int32_t process_file(const char* path, const t_flags* flags) {
  t_nm file = {};
  file.path = (uint8_t *)ft_strdup(path);
  if (file.path == NULL)
    return 1;
  if (read_file(path, &file.raw_data, &file.data_len)) {
    free(file.path);
    return print_error(path, strerror(errno));
  }
  if (file.data_len < sizeof(Elf64_Ehdr) && file.data_len < sizeof(Elf32_Ehdr)) {
    free(file.path);
    return print_error(path, "file format not recognized");
  }
  file.hdr64 = (Elf64_Ehdr *)file.raw_data;
  file.hdr32 = (Elf32_Ehdr *)file.raw_data;
  if (check_elf_header_64(file.hdr64) == 0) {
    parse_sections_64(&file);
    extract_symbols_64(&file, flags, true);
    free(file.raw_data);
    ft_lstclear(&file.lst_shdr_64, NULL);
  }
  else if (check_elf_header_32(file.hdr32) == 0) {
    parse_sections_32(&file);
    extract_symbols_32(&file, flags, false);
    free(file.raw_data);
    ft_lstclear(&file.lst_shdr_32, NULL);
  }
  else
    print_error(path, "file format not recognized");
  free(file.path);
  return 0;
}

int main(const int ac, char** av) {
  t_list* file = NULL;
  t_flags flags = {};
  flags.cmp_fn = sym_strcmp;
  flags.filter_fn = base_filter;
  if (parse_args(ac, av, &file, &flags)) {
    return 1;
  }
  if (file == NULL)
    return process_file("./a.out", &flags);
  bool format = false;
  if (ft_lstsize(file) > 1) {
    format = true;
    ft_putchar_fd('\n', 1);
  }
  for (const t_list* node = file; node; node = node->next) {
    if (!format) {
      process_file(node->content, &flags);
      continue;
    }
    ft_putstr_fd(node->content, 1);
    ft_putstr_fd(":\n", 1);
    process_file(node->content, &flags);
    if (node->next)
      ft_putchar_fd('\n', 1);
  }
  ft_lstclear(&file, free);
  return 0;
}
