//
// Created by loumouli on 11/24/23.
//

#include "ft_nm.h"





void print_value(t_nm* file, const Elf64_Sym* sym) {
  (void)file;
  if (sym->st_value == 0x0) {
    printf("                ");
  }
  else {
    printf("%016lx", sym->st_value);
  }
}

char getSymType(t_nm* file, const char* name, const Elf64_Sym* sym) {
  const int32_t bind = ELF64_ST_BIND(sym->st_info);
  const int32_t type = ELF64_ST_TYPE(sym->st_info);
  (void)name;
  if (type == STT_FILE || type == STT_SECTION) {
    return 'a';
  }
  if (bind == STB_WEAK) {
    if (sym->st_value)
      return 'W';
    return 'w';
  }
  switch (sym->st_shndx) {
    case SHN_ABS:
      return 'A';
    case SHN_UNDEF:
      return 'U';
  }
  Elf64_Shdr *shdr = (Elf64_Shdr *)(file->raw_data + file->elf64_header->e_shoff + (sym->st_shndx * file->elf64_header->e_shentsize));
  Elf64_Shdr *shstrtb = (Elf64_Shdr *)(file->raw_data + file->elf64_header->e_shoff + (file->elf64_header->e_shstrndx * file->elf64_header->e_shentsize));
  char *section_name = (char *)(file->raw_data + shstrtb->sh_offset + shdr->sh_name);
  char result = 0;
  if (ft_strncmp(section_name, ".text", ft_strlen(section_name)) == 0 )
		result = 'T';
	else if (ft_strncmp(section_name, "completed.0", ft_strlen(section_name)) == 0 )
		result = 'B';
	else if (ft_strncmp(section_name, ".fini", ft_strlen(section_name)) == 0 )
		result = 'T';
	else if (ft_strncmp(section_name, ".data", ft_strlen(section_name)) == 0 )
		result = 'D';
	else if (ft_strncmp(section_name, ".rodata", ft_strlen(section_name)) == 0 )
		result = 'R';
	else if (ft_strncmp(section_name, ".bss", ft_strlen(section_name)) == 0 )
		result = 'B';
	else if (ft_strncmp(section_name, ".init", ft_strlen(section_name)) == 0 )
		result = 'T';
	else if (ft_strncmp(section_name, ".fini_array", ft_strlen(section_name)) == 0 )
		result = 'D';
	else if (ft_strncmp(section_name, ".init_array", ft_strlen(section_name)) == 0 )
		result = 'D';
	else if (ft_strncmp(section_name, ".eh_frame", ft_strlen(section_name)) == 0 )
		result = 'R';
	else if (ft_strncmp(section_name, ".dynamic", ft_strlen(section_name)) == 0 )
		result = 'D';
	else if (ft_strncmp(section_name, ".eh_frame_hdr", ft_strlen(section_name)) == 0 )
		result = 'R';
	else if (ft_strncmp(section_name, ".got.plt", ft_strlen(section_name)) == 0 )
		result = 'D';
	else if (ft_strncmp(section_name, ".note.ABI-tag", ft_strlen(section_name)) == 0 )
		result = 'R';
  else {
    printf("%s:", section_name);
    result = '?';
  }
  if (bind == STB_LOCAL)  {
    result = tolower(result);
  }
	return result;
}

void print_type(t_nm* file, const Elf64_Sym* sym, const char* sym_str_tab) {
  const char c = getSymType(file, sym_str_tab + sym->st_name, sym);
  printf("%c", c);
}

void print_name(t_nm* file, const Elf64_Sym* sym, const char* sym_str_tab) {
  (void)file;
  printf("%s", sym_str_tab + sym->st_name);
}

void sort_entry(Elf64_Sym* sym_tab, char* sym_str_tab, const Elf64_Shdr* sym_tab_header, CmpFn cmp_fn) {
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf64_Sym); i++) {
    for (uint32_t j = i + 1; j < sym_tab_header->sh_size / sizeof(Elf64_Sym); j++) {
      if (cmp_fn(sym_tab, sym_str_tab, i, j) > 0) {
        Elf64_Sym tmp = sym_tab[i];
        sym_tab[i] = sym_tab[j];
        sym_tab[j] = tmp;
      }
    }
  }
}

int32_t extract_symbols_64(t_nm* file, t_flags *flags) {
  const Elf64_Shdr* sym_tab_header = NULL;
  const Elf64_Shdr* sym_str_tab_header = NULL;
  for (shdr_list_64_t* tmp = file->sections_list; tmp != NULL; tmp = tmp->next) {
    if (tmp->section_header->sh_type == SHT_SYMTAB) {
      sym_tab_header = tmp->section_header;
      break;
    }
  }
  if (sym_tab_header == NULL) {
    ft_putstr_fd("ft_nm: ", 2);
    ft_putstr_fd((const char *)file->path, 2);
    ft_putstr_fd(" ", 2);
    ft_putstr_fd("no symbol\n", 2);
    return 1;
  }
  uint32_t idx = 0;
  for (shdr_list_64_t* tmp = file->sections_list; tmp != NULL; tmp = tmp->next) {
    if (idx == sym_tab_header->sh_link) {
      sym_str_tab_header = tmp->section_header;
      break;
    }
    idx += 1;
  }
  if (idx != sym_tab_header->sh_link) {
    ft_putstr_fd("ft_nm: ", 2);
    ft_putstr_fd((const char *)file->path, 2);
    ft_putstr_fd(" ", 2);
    ft_putstr_fd("no symbol\n", 2);
    return 1;
  }
  Elf64_Sym* sym_tab = (Elf64_Sym *)(file->raw_data + sym_tab_header->sh_offset);
  char* sym_str_tab = (char *)(file->raw_data + sym_str_tab_header->sh_offset);
  //sort symbols in alphabetical order
  sort_entry(sym_tab, sym_str_tab, sym_tab_header, flags->cmp_fn);
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf64_Sym); i++) {
    //print info like nm
    if (!sym_tab[i].st_name) {
      continue;
    }
    //filter out symbols with name ending with .c
    if (flags->filter_fn(&sym_tab[i], sym_str_tab)) {
      continue;
    }
    print_value(file, &sym_tab[i]);
    printf(" ");
    print_type(file, &sym_tab[i], sym_str_tab);
    printf(" ");
    print_name(file, &sym_tab[i], sym_str_tab);
    printf("\n");
  }
  return 0;
}
