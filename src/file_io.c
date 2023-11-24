//
// Created by loumouli on 11/10/23.
//

#include "ft_nm.h"

//Read the entire file into memory
int32_t read_file(const char *path, uint8_t** result, size_t* len) {
  const int file = open(path, O_RDONLY);
  if (file == -1) {
    fprintf(stderr, "DEBUG: Failed opening file\n");
    return 1;
  }
  while (1) {
    uint8_t buff[8192];
    const ssize_t retval = read(file, buff, sizeof(buff));
    if (retval == -1) {
      fprintf(stderr, "DEBUG: Failed reading file\n");
      free(*result); // Free allocated memory before returning
      return 3;
    }
    if (retval == 0)
      break;
    uint8_t* new_result = realloc(*result, *len + retval); // Check realloc return value
    if (new_result == NULL) {
      fprintf(stderr, "DEBUG: Failed allocating memory\n");
      free(result); // Free previously allocated memory
      return 4; // Return a different error code
    }
    *result = new_result;
    memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}