//
// Created by loumouli on 11/10/23.
//

#include "ft_nm.h"

//Read the entire file into memory
int32_t read_file(const char* path, uint8_t** result, size_t* len) {
  const int file = open(path, O_RDONLY);
  if (file == -1) {
    return 1;
  }
  while (1) {
    uint8_t buff[8192];
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
    ft_memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}
