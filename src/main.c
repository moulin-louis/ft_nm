#include "ft_nm.h"

int32_t runtime_32(t_nm* bin);

//check health elf header from file


int32_t process_file(const char *path, t_flags* flags) {
  t_nm file = {};
  file.path = (uint8_t *)ft_strdup(path);
  if (file.path == NULL)
    return 1;
  if (read_file(path, &file.raw_data, &file.data_len)) {
    free(file.path);
    ft_putstr_fd("ft_nm: ", 2);
    ft_putstr_fd(path, 2);
    ft_putstr_fd(" ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putstr_fd("\n", 2);
    return 1;
  }
  file.elf64_header = (Elf64_Ehdr *)file.raw_data;
  if (check_elf_header_64(file.elf64_header)) {
    free(file.path);
    ft_putstr_fd("ft_nm: ", 2);
    ft_putstr_fd(path, 2);
    ft_putstr_fd(" ", 2);
    ft_putstr_fd("file format not recognized\n", 2);
    return 1;
  }
  parse_sections_64(&file);
  extract_symbols_64(&file, flags);
  free(file.raw_data);
  for (shdr_list_64_t *tmp = file.sections_list; tmp != NULL;) {
    shdr_list_64_t *next = tmp->next;
    free(tmp);
    tmp = next;
  }
  free(file.path);
  return 0;
}

void handle_sigint(int sig) {
  (void)sig;
  exit(1);
  return;
}
int main(const int ac, char** av) {
  signal(SIGINT, handle_sigint);
  t_list* file = NULL;
  t_flags flags = {0};
  flags.cmp_fn = sym_strcmp;
  flags.filter_fn = base_filter;
  if (parse_args(ac, av, &file, &flags)) {
    return 1;
  }
  t_list *tmp = NULL;
  if (file == NULL) 
    process_file("./a.out", &flags);
  else {
    for (t_list *node = file; node; node = node->next) {
      process_file(node->content, &flags);
    }
  }
  while (file) {
    tmp = file->next;
    free(file->content);
    free(file);
    file = tmp;
  }
  return 0;
}