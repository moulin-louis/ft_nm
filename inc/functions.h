//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

//I/O FUNCTION
int32_t save_new_file(const t_nm* ptr);

void print_hex(unsigned long value, int fd, const bool arch);

//PARSING FUNCTION
int32_t parse_program_headers_64(t_nm* bin);

int32_t parse_program_headers_32(t_nm* bin);

void print_parsing_error(const t_nm* nm, int32_t code);

int32_t parse_sections_64(t_nm* file);

int32_t parse_sections_32(t_nm* file);

int32_t extract_symbols_64(const t_nm* file, const t_flags* flags, bool arch);

int32_t extract_symbols_32(const t_nm* file, const t_flags* flags, bool arch);

int32_t check_elf_header_64(const t_nm* file);

int32_t check_elf_header_32(const t_nm* file);

int parse_args(int ac, char** av, t_list** head, t_flags* flags);

int32_t print_error(const char* filename, const char* error);

char getSymType_64(const t_nm* file, const Elf64_Sym* sym);

char getSymType_32(const t_nm* file, const Elf32_Sym* sym);

Elf64_Shdr* get_header_idx_64(const t_nm* file, const uint32_t shndx);

Elf32_Shdr* get_header_idx_32(const t_nm* file, const uint32_t shndx);

Elf64_Shdr* get_header_type_64(const t_nm* file, const uint64_t type);

Elf32_Shdr* get_header_type_32(const t_nm* file, const uint64_t type);

void print_lst_symbols(t_list* node);

//COMPARE FN
int sym_strcmp(const void* ptr1, const void* ptr2);

int sym_rev_strcmp(const void* ptr1, const void* ptr2);

int sym_nocmp(const void* ptr1, const void* ptr2);

//FILTER FN
int base_filter(const void* ptr);

int no_filter(const void* ptr);

int local_filter(const void* ptr);

int defined_filter(const void* ptr);

void free_sym(void* sym);
#endif //WOODY_WOODPACKER_FUNCTIONS_H
