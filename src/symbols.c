//
// Created by loumouli on 11/24/23.
//

#include <ctype.h>

#include "ft_nm.h"

// char *st_bind_to_str(uint32_t st_bind) {
// switch (st_bind) {
// case STB_LOCAL:
// return "LOCAL";
// case STB_GLOBAL:
// return "GLOBAL";
// case STB_WEAK:
// return "WEAK";
// case STB_LOOS:
// return "LOOS";
// case STB_HIOS:
// return "HIOS";
// case STB_LOPROC:
// return "LOPROC";
// case STB_HIPROC:
// return "HIPROC";
// default:
// return "UNKNOWN";
// }
// }

// char *st_type_to_str(uint32_t st_type) {
// switch (st_type) {
// case STT_NOTYPE:
// return "NOTYPE";
// case STT_OBJECT:
// return "OBJECT";
// case STT_FUNC:
// return "FUNC";
// case STT_SECTION:
// return "SECTION";
// case STT_FILE:
// return "FILE";
// case STT_COMMON:
// return "COMMON";
// case STT_TLS:
// return "TLS";
// case STT_LOOS:
// return "LOOS";
// case STT_HIOS:
// return "HIOS";
// case STT_LOPROC:
// return "LOPROC";
// case STT_HIPROC:
// return "HIPROC";
// default:
// return "UNKNOWN";
// }
// }

// char *st_other_to_str(uint32_t st_other) {
// switch (st_other) {
// case STV_DEFAULT:
// return "DEFAULT";
// case STV_INTERNAL:
// return "INTERNAL";
// case STV_HIDDEN:
// return "HIDDEN";
// case STV_PROTECTED:
// return "PROTECTED";
// case 0x04:
// return "EXPORTED";
// case 0x05:
// return "SINGLETON";
// case 0x06:
// return "ELIMINATE";
// default:
// return "UNKNOWN";
// }
// }

char* to_lower(char* str) {
  for (uint32_t i = 0; i < strlen(str); i++) {
    str[i] = tolower(str[i]);
  }
  return str;
}

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
  if (stringcmp(section_name, ".text") == 0 && bind == STB_GLOBAL)
		return 'T';
	else if (stringcmp(section_name, ".text") == 0 && bind == STB_LOCAL)
		return 't';
	else if (stringcmp(section_name, "completed.0") == 0 && bind == STB_GLOBAL)
		return 'B';
	else if (stringcmp(section_name, "completed.0") == 0 && bind == STB_LOCAL)
		return 'b';
	else if (stringcmp(section_name, ".fini") == 0 && bind == STB_GLOBAL)
		return 'T';
	else if (stringcmp(section_name, ".fini") == 0 && bind == STB_LOCAL)
		return 't';
	else if (stringcmp(section_name, ".data") == 0 && bind == STB_GLOBAL)
		return 'D';
	else if (stringcmp(section_name, ".data") == 0 && bind == STB_LOCAL)
		return 'd';
	else if (stringcmp(section_name, ".rodata") == 0 && bind == STB_GLOBAL)
		return 'R';
	else if (stringcmp(section_name, ".rodata") == 0 && bind == STB_LOCAL)
		return 'r';
	else if (stringcmp(section_name, ".bss") == 0 && bind == STB_GLOBAL)
		return 'B';
	else if (stringcmp(section_name, ".bss") == 0 && bind == STB_LOCAL)
		return 'b';
	else if (stringcmp(section_name, ".init") == 0 && bind == STB_GLOBAL)
		return 'T';
	else if (stringcmp(section_name, ".init") == 0 && bind == STB_LOCAL)
		return 't';
	else if (stringcmp(section_name, ".fini_array") == 0 && bind == STB_GLOBAL)
		return 'D';
	else if (stringcmp(section_name, ".fini_array") == 0 && bind == STB_LOCAL)
		return 'd';
	else if (stringcmp(section_name, ".init_array") == 0 && bind == STB_GLOBAL)
		return 'D';
	else if (stringcmp(section_name, ".init_array") == 0 && bind == STB_LOCAL)
		return 'd';
	else if (stringcmp(section_name, ".eh_frame") == 0 && bind == STB_GLOBAL)
		return 'R';
	else if (stringcmp(section_name, ".eh_frame") == 0 && bind == STB_LOCAL)
		return 'r';
	else if (stringcmp(section_name, ".dynamic") == 0 && bind == STB_GLOBAL)
		return 'D';
	else if (stringcmp(section_name, ".dynamic") == 0 && bind == STB_LOCAL)
		return 'd';
	else if (stringcmp(section_name, ".eh_frame_hdr") == 0 && bind == STB_GLOBAL)
		return 'R';
	else if (stringcmp(section_name, ".eh_frame_hdr") == 0 && bind == STB_LOCAL)
		return 'r';
	else if (stringcmp(section_name, ".got.plt") == 0 && bind == STB_GLOBAL)
		return 'D';
	else if (stringcmp(section_name, ".got.plt") == 0 && bind == STB_LOCAL)
		return 'd';
	else if (stringcmp(section_name, ".note.ABI-tag") == 0 && bind == STB_GLOBAL)
		return 'R';
	else if (stringcmp(section_name, ".note.ABI-tag") == 0 && bind == STB_LOCAL)
		return 'r';

	else
		printfmt(STDERR_FILENO, "section_name: %s\n", section_name);

	return '?';
}

void print_type(t_nm* file, const Elf64_Sym* sym, const char* sym_str_tab) {
  const char c = getSymType(file, sym_str_tab + sym->st_name, sym);
  printf("%c", c);
}

void print_name(t_nm* file, const Elf64_Sym* sym, const char* sym_str_tab) {
  (void)file;
  printf("%s", sym_str_tab + sym->st_name);
}

int32_t extract_symbols_64(t_nm* file) {
  const Elf64_Shdr* sym_tab_header = NULL;
  const Elf64_Shdr* sym_str_tab_header = NULL;
  for (shdr_list_64_t* tmp = file->sections_list; tmp != NULL; tmp = tmp->next) {
    if (tmp->section_header->sh_type == SHT_SYMTAB) {
      sym_tab_header = tmp->section_header;
      break;
    }
  }
  if (sym_tab_header == NULL) {
    fprintf(stderr, "ft_nm: %s: %s\n", (char *)file->path, "no symbols");
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
    fprintf(stderr, "ft_nm: %s: %s\n", (char *)file->path, "no symbols");
    return 1;
  }
  Elf64_Sym* sym_tab = (Elf64_Sym *)(file->raw_data + sym_tab_header->sh_offset);
  char* sym_str_tab = (char *)(file->raw_data + sym_str_tab_header->sh_offset);
  //sort symbols in alphabetical order
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf64_Sym); i++) {
    for (uint32_t j = i + 1; j < sym_tab_header->sh_size / sizeof(Elf64_Sym); j++) {
      uint32_t idx_start1 = 0;
      uint32_t idx_start2 = 0;
      while (sym_str_tab[sym_tab[i].st_name + idx_start1] == '_') {
        idx_start1 += 1;
      }
      while (sym_str_tab[sym_tab[j].st_name + idx_start2] == '_') {
        idx_start2 += 1;
      }
      const char* str1 = to_lower(strdup(sym_str_tab + sym_tab[i].st_name + idx_start1));
      const char* str2 = to_lower(strdup(sym_str_tab + sym_tab[j].st_name + idx_start2));
      if (strcmp(str1, str2) > 0) {
        Elf64_Sym tmp = sym_tab[i];
        sym_tab[i] = sym_tab[j];
        sym_tab[j] = tmp;
      }
      free((void *)str1);
      free((void *)str2);
    }
  }
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf64_Sym); i++) {
    //print info like nm
    if (!sym_tab[i].st_name) {
      continue;
    }
    //filter out symbols with name ending with .c
    if (sym_str_tab[sym_tab[i].st_name + strlen(sym_str_tab + sym_tab[i].st_name) - 2] == '.' && (
          sym_str_tab[sym_tab[i].st_name + strlen(sym_str_tab + sym_tab[i].st_name) - 1] == 'c' || sym_str_tab[
            sym_tab[i].st_name + strlen(sym_str_tab + sym_tab[i].st_name) - 1] == 'o')) {
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
