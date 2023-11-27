//
// Created by loumouli on 11/27/23.
//

#include "ft_nm.h"

char		_64_print_SymType_suite(const t_nm* file, Elf64_Sym* sym);
char		_64_print_SymType_sec_suite(const t_nm* file, Elf64_Sym* sym);

char		_64_print_SymType(const t_nm* file, Elf64_Sym* sym) {
  char		c;

  if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
    c = 'u';
  else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
    {
      c = 'W';
      if (sym->st_shndx == SHN_UNDEF)
        c = 'w';
    }
  else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK && ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
    {
      c = 'V';
      if (sym->st_shndx == SHN_UNDEF)
        c = 'v';
    }
  else if (sym->st_shndx == SHN_UNDEF)
    c = 'U';
  else
    return (_64_print_SymType_suite(file, sym));
  if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL)
    c += 32;
  return (c);
}

char		_64_print_SymType_suite(const t_nm* file, Elf64_Sym* sym) {
  char		c;

  Elf64_Shdr* section_header = get_header_idx_64(file, sym->st_shndx);
  if (sym->st_shndx == SHN_ABS)
    c = 'A';
  else if (sym->st_shndx == SHN_COMMON)
    c = 'C';
  else if (section_header->sh_type == SHT_NOBITS && section_header->sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'B';
  else if (section_header->sh_type == SHT_PROGBITS && section_header->sh_flags == SHF_ALLOC)
    c = 'R';
  else
    return (_64_print_SymType_sec_suite(file, sym));
  if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL)
    c += 32;
  return (c);
}

char		_64_print_SymType_sec_suite(const t_nm* file, Elf64_Sym* sym) {
  char		c;

  Elf64_Shdr* section_header = get_header_idx_64(file, sym->st_shndx);
  if (section_header->sh_type == SHT_PROGBITS && section_header->sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'D';
  else if (section_header->sh_type == SHT_PROGBITS && section_header->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
    c = 'T';
  else if (section_header->sh_type == SHT_DYNAMIC)
    c = 'D';
  else
    c = ('t' - 32);
  if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
    c += 32;
  return (c);
}

char get_symbol_type(const t_nm* file, const Elf64_Sym* sym) {
  unsigned char type = ELF64_ST_TYPE(sym->st_info);
  unsigned char bind = ELF64_ST_BIND(sym->st_info);

  // Undefined symbol
  if (sym->st_shndx == SHN_UNDEF) {
    return 'U';
  }

  // Absolute symbol
  if (sym->st_shndx == SHN_ABS) {
    return 'A';
  }

  // Common symbol
  if (type == STT_COMMON) {
    return 'C';
  }

  // Section type and other special cases
  switch (get_header_idx_64(file, sym->st_shndx)->sh_type) {
    case SHT_PROGBITS:
      if (get_header_idx_64(file, sym->st_shndx)->sh_flags & SHF_ALLOC) {
        if (get_header_idx_64(file, sym->st_shndx)->sh_flags & SHF_WRITE) {
          // Initialized data section
          return bind == STB_LOCAL ? 'd' : 'D';
        } else if (get_header_idx_64(file, sym->st_shndx)->sh_flags & SHF_EXECINSTR) {
          // Code section
          return bind == STB_LOCAL ? 't' : 'T';
        } else {
          // Read-only data section
          return bind == STB_LOCAL ? 'r' : 'R';
        }
      }
    break;

    case SHT_NOBITS:
      // Uninitialized data section
        return bind == STB_LOCAL ? 'b' : 'B';

    default:
      break;
  }

  // Weak symbols
  if (bind == STB_WEAK) {
    return type == STT_OBJECT ? 'V' : 'W';
  }

  // Other cases
  return '?';  // Unknown type
}

char getSymType(const t_nm* file, const Elf64_Sym* sym) {
  // return get_symbol_type(file, sym);
  const int32_t bind = ELF64_ST_BIND(sym->st_info);
  const int32_t type = ELF64_ST_TYPE(sym->st_info);
  if (type == STT_FILE){
    return 'a';
  }
  if (type == STT_SECTION) {
    return 'n';
  }
  if (bind == STB_WEAK) {
    if (type == STT_OBJECT) {
      if (sym->st_shndx == SHN_UNDEF)
        return 'v';
      return 'V';
    }
    if (sym->st_shndx == SHN_UNDEF)
      return 'w';
    return 'W';
  }
  switch (sym->st_shndx) {
    case SHN_ABS:
      return 'A';
    case SHN_UNDEF:
      return 'U';
  }
  const Elf64_Shdr* shdr = (Elf64_Shdr *)(file->raw_data + file->elf64_header->e_shoff + sym->st_shndx * file->
                                          elf64_header->e_shentsize);
  const Elf64_Shdr* shstrtb = (Elf64_Shdr *)(file->raw_data + file->elf64_header->e_shoff + file->elf64_header->
                                             e_shstrndx * file->elf64_header->e_shentsize);
  const char* section_name = (char *)(file->raw_data + shstrtb->sh_offset + shdr->sh_name);
  char result;
  const size_t len = ft_strlen(section_name);
  if (ft_strncmp(section_name, ".text", len) == 0)
    result = 'T';
  else if (ft_strncmp(section_name, "completed.0", len) == 0)
    result = 'B';
  else if (ft_strncmp(section_name, ".fini", len) == 0)
    result = 'T';
  else if (ft_strncmp(section_name, ".data", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".data.rel.ro", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".rodata", len) == 0)
    result = 'R';
  else if (ft_strncmp(section_name, ".bss", len) == 0)
    result = 'B';
  else if (ft_strncmp(section_name, ".tbss", len) == 0)
    result = 'B';
  else if (ft_strncmp(section_name, ".init", len) == 0)
    result = 'T';
  else if (ft_strncmp(section_name, ".fini_array", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".init_array", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".eh_frame", len) == 0)
    result = 'R';
  else if (ft_strncmp(section_name, ".dynamic", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".eh_frame_hdr", len) == 0)
    result = 'R';
  else if (ft_strncmp(section_name, ".got.plt", len) == 0)
    result = 'D';
  else if (ft_strncmp(section_name, ".note.ABI-tag", len) == 0)
    result = 'R';
  else {
    result = '?';
  }
  if (bind == STB_LOCAL) {
    result = tolower(result);
  }
  return result;
}

