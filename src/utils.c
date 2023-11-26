//
// Created by loumouli on 11/10/23.
//

#include "ft_nm.h"

//Read the entire file into memory
int32_t read_file(const char* path, uint8_t** result, size_t* len) {
  uint8_t buff[32768];
  const int file = open(path, O_RDONLY);
  if (file == -1) {
    return 1;
  }
  while (1) {
    const ssize_t retval = read(file, buff, sizeof(buff));
    if (retval == -1) {
      free(*result);
      return 3;
    }
    if (retval == 0)
      break;
    uint8_t* new_result = realloc(*result, *len + retval);
    if (new_result == NULL) {
      free(result);
      return 4;
    }
    *result = new_result;
    memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}

int32_t print_error(const char *filename, const char *error) {
  ft_putstr_fd("ft_nm: ", 2);
  ft_putstr_fd(filename, 2);
  ft_putstr_fd(" ", 2);
  ft_putstr_fd(error, 2);
  ft_putstr_fd("\n", 2);
  return 1;
}

__attribute__((unused)) void hexdump(void* data, size_t len, int32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      printf("%02x ", ((uint8_t *)data)[i]);
    }
    printf("\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      printf("%02x ", ((uint8_t *)data)[j]);
    }
    printf("\n");
  }
  printf("\n");
}

__attribute__((unused)) void asciidump(void* data, size_t len, uint32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      if (((uint8_t *)data)[i] >= 0x20 && ((uint8_t *)data)[i] <= 0x7e) {
        fprintf(stdout, "%c", ((uint8_t *)data)[i]);
      }
      else {
        fprintf(stdout, ".");
      }
    }
    fprintf(stdout, "\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      if (((uint8_t *)data)[j] >= 0x20 && ((uint8_t *)data)[j] <= 0x7e) {
        fprintf(stdout, "%c", ((uint8_t *)data)[j]);
      }
      else {
        fprintf(stdout, ".");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
}
