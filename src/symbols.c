//
// Created by loumouli on 11/24/23.
//

#include "ft_nm.h"

int32_t add_sym_node(
  const t_nm* file,
  t_list** head,
  void* data,
  const char* sym_str_tab,
  const bool dyn_located,
  const bool arch
) {
  t_list* node = malloc(sizeof(t_list));
  if (node == NULL)
    return 1;
  node->next = NULL;
  node->content = ft_calloc(sizeof(t_sym), 1);
  if (node->content == NULL) {
    free(node);
    return 1;
  }
  ((t_sym *)node->content)->arch = arch;
  // CHECK IF THIS IS 64 BITS OR 32 BITS SYMBOLS/FILE/ETC
  if (arch) {
    ((t_sym *)node->content)->name = ft_strdup(sym_str_tab + ((Elf64_Sym *)data)->st_name);
    ((t_sym *)node->content)->len_name = ft_strlen(sym_str_tab + ((Elf64_Sym *)data)->st_name);
    ((t_sym *)node->content)->type = getSymType_64(file, data);
    ((t_sym *)node->content)->value = ((Elf64_Sym *)data)->st_value;
  }
  else {
    ((t_sym *)node->content)->name = ft_strdup(sym_str_tab + ((Elf32_Sym *)data)->st_name);
    ((t_sym *)node->content)->len_name = ft_strlen(sym_str_tab + ((Elf32_Sym *)data)->st_name);
    ((t_sym *)node->content)->type = getSymType_32(file, data);
    ((t_sym *)node->content)->value = ((Elf32_Sym *)data)->st_value;
  }
  if (((t_sym *)node->content)->name == NULL) {
    free(node->content);
    free(node);
    return 1;
  }
  while (((t_sym *)node->content)->name[((t_sym *)node->content)->idx_start] == '_') {
    ((t_sym *)node->content)->idx_start += 1;
  }
  ((t_sym *)node->content)->dyn_located = dyn_located;
  ((t_sym *)node->content)->sym = data;
  // ADDING NODE TO THE LIST
  if (*head == NULL) {
    node->last_node = node;
    *head = node;
  }
  else {
    (*head)->last_node->next = node;
    (*head)->last_node = node;
  }
  return 0;
}

t_list* craft_linked_list_64(
  const t_nm* file,
  const Elf64_Shdr* sym_tab_header,
  const Elf64_Shdr* sym_str_tab_header,
  const Elf64_Shdr* dyn_sym_header,
  const Elf64_Shdr* dyn_str_tab_header,
  const bool arch
) {
  const char* sym_str_tab = (char *)(file->raw_data + sym_str_tab_header->sh_offset);
  t_list* result = NULL;
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf64_Sym); i++) {
    Elf64_Sym* sym = (Elf64_Sym *)(file->raw_data + sym_tab_header->sh_offset + i * sizeof(Elf64_Sym));
    if (add_sym_node(file, &result, sym, sym_str_tab, false, arch))
      return NULL;
  }
  if (dyn_sym_header == NULL)
    return result;
  const char* dyn_str_tab = (char *)(file->raw_data + dyn_str_tab_header->sh_offset);
  for (uint32_t i = 0; i < dyn_sym_header->sh_size / sizeof(Elf64_Sym); i++) {
    Elf64_Sym* sym = (Elf64_Sym *)(file->raw_data + dyn_sym_header->sh_offset + i * sizeof(Elf64_Sym));
    if (add_sym_node(file, &result, sym, dyn_str_tab, true, arch))
      return NULL;
  }
  return result;
}

t_list* craft_linked_list_32(
  const t_nm* file,
  const Elf32_Shdr* sym_tab_header,
  const Elf32_Shdr* sym_str_tab_header,
  const Elf32_Shdr* dyn_sym_header,
  const Elf32_Shdr* dyn_str_tab_header,
  const bool arch
) {
  const char* sym_str_tab = (char *)(file->raw_data + sym_str_tab_header->sh_offset);
  t_list* result = NULL;
  for (uint32_t i = 0; i < sym_tab_header->sh_size / sizeof(Elf32_Sym); i++) {
    Elf32_Sym* sym = (Elf32_Sym *)(file->raw_data + sym_tab_header->sh_offset + i * sizeof(Elf32_Sym));
    if (add_sym_node(file, &result, sym, sym_str_tab, false, arch))
      return NULL;
  }
  if (dyn_sym_header == NULL)
    return result;
  const char* dyn_str_tab = (char *)(file->raw_data + dyn_str_tab_header->sh_offset);
  for (uint32_t i = 0; i < dyn_sym_header->sh_size / sizeof(Elf32_Sym); i++) {
    Elf32_Sym* sym = (Elf32_Sym *)(file->raw_data + dyn_sym_header->sh_offset + i * sizeof(Elf32_Sym));
    if (add_sym_node(file, &result, sym, dyn_str_tab, true, arch))
      return NULL;
  }
  return result;
}

void display_symbols(const t_list* head) {
  if (head == NULL)
    return;
  const bool arch = ((t_sym *)head->content)->arch;
  for (const t_list* tmp = head; tmp != NULL; tmp = tmp->next) {
    if (((t_sym *)tmp->content)->type == 'U'
        || (((t_sym *)tmp->content)->value == 0x0 && ((t_sym *)tmp->content)->type != 'a' && ((t_sym *)tmp->content)->
            type != 'T' && ((t_sym *)tmp->content)->type != 'b'))
      ft_putstr_fd("                ", 1);
    else {
      print_hex(((t_sym *)tmp->content)->value, 1, arch);
    }
    ft_putchar_fd(' ', 1);
    ft_putchar_fd(((t_sym *)tmp->content)->type, 1);
    ft_putchar_fd(' ', 1);
    ft_putstr_fd(((t_sym *)tmp->content)->name, 1);
    ft_putchar_fd('\n', 1);
  }
}

int32_t extract_symbols_64(const t_nm* file, const t_flags* flags, const bool arch) {
  const Elf64_Shdr* sym_tab_hdr = get_header_type_64(file, SHT_SYMTAB);
  if (!sym_tab_hdr)
    return print_error((const char *)file->path, "no symbols");
  const Elf64_Shdr* sym_str_tab_hdr = get_header_idx_64(file, sym_tab_hdr->sh_link);
  if (sym_str_tab_hdr == NULL)
    return print_error((const char *)file->path, "no symbols");
  const Elf64_Shdr* dyn_sym_hdr = get_header_type_64(file, SHT_DYNSYM);
  const Elf64_Shdr* dyn_str_tab_hdr = NULL;
  if (dyn_sym_hdr)
    dyn_str_tab_hdr = get_header_idx_64(file, dyn_sym_hdr->sh_link);
  t_list* head = craft_linked_list_64(file, sym_tab_hdr, sym_str_tab_hdr, dyn_sym_hdr, dyn_str_tab_hdr, arch);
  filter_lst(&head, flags->filter_fn, free_sym);
  ft_lstsort(head, flags->cmp_fn);
  display_symbols(head);
  ft_lstclear(&head, free_sym);
  return 0;
}

int32_t extract_symbols_32(const t_nm* file, const t_flags* flags, const bool arch) {
  const Elf32_Shdr* sym_tab_hdr = get_header_type_32(file, SHT_SYMTAB);
  if (!sym_tab_hdr)
    return print_error((const char *)file->path, "no symbols");
  const Elf32_Shdr* sym_str_tab_hdr = get_header_idx_32(file, sym_tab_hdr->sh_link);
  if (sym_str_tab_hdr == NULL)
    return print_error((const char *)file->path, "no symbols");
  const Elf32_Shdr* dyn_sym_hdr = get_header_type_32(file, SHT_DYNSYM);
  const Elf32_Shdr* dyn_str_tab_hdr = NULL;
  if (dyn_sym_hdr)
    dyn_str_tab_hdr = get_header_idx_32(file, dyn_sym_hdr->sh_link);
  t_list* head = craft_linked_list_32(file, sym_tab_hdr, sym_str_tab_hdr, dyn_sym_hdr, dyn_str_tab_hdr, arch);
  filter_lst(&head, flags->filter_fn, free_sym);
  ft_lstsort(head, flags->cmp_fn);
  display_symbols(head);
  ft_lstclear(&head, free_sym);
  return 0;
}
