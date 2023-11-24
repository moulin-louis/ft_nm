#include "ft_nm.h"

int32_t runtime_32(t_nm* bin);

//check health elf header from file
int32_t check_elf_header_64(const Elf64_Ehdr* elf64Ehdr) {
  //check for the magic number
  if (memcmp(elf64Ehdr, "\x7F" "ELF", 4) != 0) {
    fprintf(stderr, "DEBUG: This is not an ELF file\n");
    return 1;
  }
  //check for the elf version
  if (elf64Ehdr->e_version != 1) {
    fprintf(stderr, "DEBUG: Wrong ELF version\n");
    return 1;
  }
  //check for the elf type
  if (elf64Ehdr->e_type != ET_EXEC && elf64Ehdr->e_type != ET_DYN && elf64Ehdr->e_type != ET_REL) {
    fprintf(stderr, "DEBUG: Wront e_type !\n");
    return 1;
  }
  return 0;
}

int32_t process_file(const char *path) {
  t_nm file = {};
  file.path = (uint8_t *)strdup(path);
  if (file.path == NULL) {
    return 1;
  }
  if (read_file(path, &file.raw_data, &file.data_len)) {
    fprintf(stderr, "ft_nm: %s: %s\n", path, strerror(errno));
    return 1;
  }
  file.elf64_header = (Elf64_Ehdr *)file.raw_data;
  if (check_elf_header_64(file.elf64_header)) {
    fprintf(stderr, "ft_nm: %s: %s\n", path, "file format not recognized");
    return 1;
  }
  parse_sections_64(&file);
  extract_symbols_64(&file);
  free(file.raw_data);
  for (shdr_list_64_t *tmp = file.sections_list; tmp != NULL;) {
    shdr_list_64_t *next = tmp->next;
    free(tmp);
    tmp = next;
  }
  free(file.path);
  return 0;
}

int main(const int ac, char** av) {
  if (ac == 1) {
    return process_file("./a.out");
  }

  for (int i = 1; i < ac; i++) {
    if (process_file(av[i])) {
      return 1;
    }
  }
  return 0;
}

int32_t runtime_32(t_nm* bin) {
  (void)bin;
  // printf(ANSI_RED "WARNING: THIS IS A 32 BITS ELF\n" ANSI_RESET);
  // printf(ANSI_GREEN "LOG: Health check.............: ");
  // bin->elf32_header = (Elf32_Ehdr *)bin->raw_data;
  // if (check_elf_header_32(bin->elf32_header)) {
  // printf(ANSI_RED ANSI_CROSS "\n");
  // fprintf(stderr, "Error checking elf header\n" ANSI_RESET);
  // return 1;
  // }
  // printf(ANSI_CHECK "\n" ANSI_RESET);

  // parse program headers
  // printf(ANSI_GREEN "LOG: Parsing headers..........: ");
  // if (parse_program_headers_32(bin)) {
  // printf(ANSI_RED ANSI_CROSS "\n");
  // fprintf(stderr, "Error parsing program headers\n" ANSI_RESET);
  // return 1;
  // }
  // printf(ANSI_CHECK "\n" ANSI_RESET);

  // cleanup
  // printf(ANSI_GREEN "LOG: Cleaning up ressources...: ");
  // free(bin->raw_data);
  // for (phdr_list_32_t* tmp = bin->phdrs_32; tmp != NULL;) {
  // phdr_list_32_t* next = tmp->next;
  // free(tmp);
  // tmp = next;
  // }
  // printf(ANSI_CHECK ANSI_RESET "\n");

  return 0;
}
