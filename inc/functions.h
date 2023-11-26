//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

//I/O FUNCTION
int32_t read_file(const char *path, uint8_t** result, uint64_t* len);

int32_t save_new_file(const t_nm* ptr);

//PARSING FUNCTION
int32_t parse_program_headers_64(t_nm* bin);
int32_t parse_program_headers_32(t_nm* bin);

//DEBUG FUNCTIONS
void print_dyn_tag_64(const Elf64_Dyn* tag);
void print_dyn_tag_32(const Elf32_Dyn* tag);

__attribute__((unused)) void hexdump(void* data, uint64_t len, int32_t row);
__attribute__((unused)) void asciidump(void *data, size_t len, uint32_t row);
__attribute__((unused)) void print_elf_header_32(Elf32_Ehdr* header);
__attribute__((unused)) void print_elf_header_64(Elf64_Ehdr* header);

int32_t parse_sections_64(t_nm* file);
int32_t extract_symbols_64(const t_nm* file, const t_flags* flags);
int32_t check_elf_header_64(const Elf64_Ehdr* elf64Ehdr);
int parse_args(int ac, char** av, t_list** head, t_flags* flags);

//COMPARE FN
int sym_strcmp(const Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j);
int sym_rev_strcmp(const Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j);
int sym_nocmp(const Elf64_Sym* sym_tab, const char* sym_str_tab, int i, int j);

//FILTER FN
int base_filter(Elf64_Sym* sym_tab, const char* sym_str_tab);
int no_filter(Elf64_Sym* sym_tab, const char* sym_str_tab);
int local_filter(Elf64_Sym* sym_tab, const char* sym_str_tab);
int defined_filter(Elf64_Sym* sym_tab, const char* sym_str_tab);
#endif //WOODY_WOODPACKER_FUNCTIONS_H
