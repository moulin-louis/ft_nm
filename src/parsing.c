//
// Created by loumouli on 11/24/23.
//

#include "ft_nm.h"

int32_t sections_push_back_64(t_nm* file, Elf64_Shdr* segment) {
  shdr_list_64_t* node = calloc(sizeof(shdr_list_64_t), 1);
  if (node == NULL)
    return 1;
  node->section_header = segment;
  if (file->sections_list == NULL) {
    file->sections_list = node;
    return 0;
  }
  shdr_list_64_t* tmp = file->sections_list;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = node;
  return 0;
}

int32_t parse_sections_64(t_nm* file) {
  const uint64_t offset_section_table = file->elf64_header->e_shoff;
  uint8_t* sections_table = file->raw_data + offset_section_table;
  for (uint32_t idx = 0; idx < (file->elf64_header->e_shnum * file->elf64_header->e_shentsize); idx += file->elf64_header->e_shentsize) {
    if (sections_push_back_64(file, (Elf64_Shdr *)(sections_table + idx))) {
      return 1;
    }
  }
  return 0;
}
