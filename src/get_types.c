//
// Created by loumouli on 11/27/23.
//

#include "ft_nm.h"

char getSymType_64(const t_nm* file, const Elf64_Sym* sym) {
  const int32_t bind = ELF64_ST_BIND(sym->st_info);
  const int32_t type = ELF64_ST_TYPE(sym->st_info);
  if (type == STT_FILE)
    return 'a';
  if (type == STT_SECTION)
    return 'n';
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
  const Elf64_Shdr* shdr = (Elf64_Shdr*)(file->raw_data + file->hdr64->e_shoff + sym->st_shndx * file->
    hdr64->e_shentsize);
  const Elf64_Shdr* shstrtb = (Elf64_Shdr*)(file->raw_data + file->hdr64->e_shoff + file->hdr64->
    e_shstrndx * file->hdr64->e_shentsize);
  const char* section_name = (char*)(file->raw_data + shstrtb->sh_offset + shdr->sh_name);
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
  else if (ft_strncmp(section_name, ".jcr", len) == 0)
    result = 'd';
  else
    result = '?';
  if (bind == STB_LOCAL)
    result = ft_tolower(result);
  return result;
}

char getSymType_32(const t_nm* file, const Elf32_Sym* sym) {
  const int32_t bind = ELF32_ST_BIND(sym->st_info);
  const int32_t type = ELF32_ST_TYPE(sym->st_info);
  if (type == STT_FILE)
    return 'a';
  if (type == STT_SECTION)
    return 'n';
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
  const Elf32_Shdr* shdr = (Elf32_Shdr*)(file->raw_data + file->hdr32->e_shoff + sym->st_shndx * file->
    hdr32->e_shentsize);
  const Elf32_Shdr* shstrtb = (Elf32_Shdr*)(file->raw_data + file->hdr32->e_shoff + file->hdr32->
    e_shstrndx * file->hdr32->e_shentsize);
  const char* section_name = (char*)(file->raw_data + shstrtb->sh_offset + shdr->sh_name);
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
  else if (ft_strncmp(section_name, ".jcr", len) == 0)
    return 'd';
  else
    result = '?';
  if (bind == STB_LOCAL)
    result = ft_tolower(result);
  return result;
}
