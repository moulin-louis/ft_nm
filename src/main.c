#include <stdio.h>

#include "ft_nm.h"

static int32_t check_64(t_nm* file, const t_flags* flags) {
  file->hdr64 = (Elf64_Ehdr*)file->raw_data;
  const int32_t retval = check_elf_header_64(file);
  if (retval) {
    print_parsing_error(file, retval);
    return retval;
  }
  parse_sections_64(file);
  extract_symbols_64(file, flags, true);
  return 0;
}

static int32_t check_32(t_nm* file, const t_flags* flags) {
  file->hdr32 = (Elf32_Ehdr*)file->raw_data;
  const int32_t retval = check_elf_header_32(file);
  if (retval) {
    print_parsing_error(file, retval);
    if (retval == 3 || retval == 5)
      print_error((const char *)file->path, "file format not recognized");
    return retval;
  }
  parse_sections_32(file);
  extract_symbols_32(file, flags, false);
  return 0;
}

//check health elf header from file
int32_t process_file(const char* path, const t_flags* flags) {
  t_nm file = {};

  memset(&file, 0, sizeof(file));
  file.path = (uint8_t*)path;
  if (read_file(path, &file.raw_data, &file.data_len))
    return print_error(path, strerror(errno));
  if (check_64(&file, flags) == 0)
    goto end;
  check_32(&file, flags);
end:
  free(file.raw_data);
  return 0;
}

int main(const int ac, char** av) {
  t_list* file = NULL;
  t_flags flags = {};
  flags.cmp_fn = sym_strcmp;
  flags.filter_fn = base_filter;
  if (parse_args(ac, av, &file, &flags))
    return 1;
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
