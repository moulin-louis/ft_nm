//
// Created by loumouli on 11/24/23.
//

#include "ft_nm.h"

int32_t parse_sections_64(t_nm* file) {
  const uint64_t offset_section_table = file->hdr64->e_shoff;
  uint8_t* sections_table = file->raw_data + offset_section_table;
  for (uint32_t idx = 0; idx < file->hdr64->e_shnum * file->hdr64->e_shentsize; idx += file->hdr64->e_shentsize) {
    t_list* node = ft_lstnew(sections_table + idx);
    if (node == NULL)
      return 1;
    ft_lstadd_back(&file->lst_shdr_64, node);
  }
  return 0;
}

int32_t parse_sections_32(t_nm* file) {
  const uint64_t offset_section_table = file->hdr32->e_shoff;
  uint8_t* sections_table = file->raw_data + offset_section_table;
  for (uint32_t idx = 0; idx < file->hdr32->e_shnum * file->hdr32->e_shentsize; idx += file->hdr32->e_shentsize) {
    t_list* node = ft_lstnew(sections_table + idx);
    if (node == NULL)
      return 1;
    ft_lstadd_back(&file->lst_shdr_32, node);
  }
  return 0;
}

int32_t check_elf_header_64(const Elf64_Ehdr* elf64Ehdr) {
  if (ft_memcmp(elf64Ehdr, "\x7F" "ELF", 4) != 0)
    return 1;
  if (elf64Ehdr->e_version != 1)
    return 1;
  if (elf64Ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    return 1;
  if (elf64Ehdr->e_type != ET_EXEC && elf64Ehdr->e_type != ET_DYN && elf64Ehdr->e_type != ET_REL)
    return 1;
  return 0;
}

int32_t check_elf_header_32(const Elf32_Ehdr* elf32Ehdr) {
  if (ft_memcmp(elf32Ehdr, "\x7F" "ELF", 4) != 0)
    return 1;
  if (elf32Ehdr->e_version != 1)
    return 1;
  if (elf32Ehdr->e_ident[EI_CLASS] != ELFCLASS32)
    return 1;
  if (elf32Ehdr->e_type != ET_EXEC && elf32Ehdr->e_type != ET_DYN && elf32Ehdr->e_type != ET_REL)
    return 1;
  return 0;
}

int is_flags(const char* args) {
  const size_t len = ft_strlen(args);
  if (
    ft_strncmp(args, "-a", len) == 0 ||
    ft_strncmp(args, "--debug-syms", len) == 0 ||
    ft_strncmp(args, "-g", len) == 0 ||
    ft_strncmp(args, "--extern-only", len) == 0 ||
    ft_strncmp(args, "-u", len) == 0 ||
    ft_strncmp(args, "--undefined-only", len) == 0 ||
    ft_strncmp(args, "-r", len) == 0 ||
    ft_strncmp(args, "--reverse-sort", len) == 0 ||
    ft_strncmp(args, "-p", len) == 0 ||
    ft_strncmp(args, "--no-sort", len) == 0
  ) {
    return 1;
  }
  return 0;
}

void process_flags(const char* args, t_flags* flags) {
  const size_t len = ft_strlen(args);
  if (ft_strncmp(args, "-r", len) == 0 || ft_strncmp(args, "--reverse-sort", len) == 0)
    flags->cmp_fn = sym_rev_strcmp;
  if (ft_strncmp(args, "-p", len) == 0 || ft_strncmp(args, "--no-sort", len) == 0)
    flags->cmp_fn = NULL;
  if (ft_strncmp(args, "-a", len) == 0 || ft_strncmp(args, "--debug-syms", len) == 0)
    flags->filter_fn = no_filter;
  if (ft_strncmp(args, "-g", len) == 0 || ft_strncmp(args, "--extern-only", len) == 0)
    flags->filter_fn = local_filter;
  if (ft_strncmp(args, "-u", len) == 0 || ft_strncmp(args, "--undefined-only", len) == 0)
    flags->filter_fn = defined_filter;
}

int add_to_head(t_list** head, const char* args) {
  t_list* node = ft_calloc(sizeof(t_list), 1);
  if (node == NULL)
    return 1;
  node->content = ft_strdup(args);
  if (node->content == NULL) {
    free(node);
    return 1;
  }
  if (*head == NULL) {
    *head = node;
    return 0;
  }
  t_list* tmp = *head;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = node;
  return 0;
}

int parse_args(const int ac, char** av, t_list** head, t_flags* flags) {
  for (int idx = 1; idx < ac; idx++) {
    if (is_flags(av[idx])) {
      process_flags(av[idx], flags);
      continue;
    }
    if (add_to_head(head, av[idx]))
      return 1;
  }
  return 0;
}
