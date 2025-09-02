#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *content = (char *)malloc(file_size + 1);
  if (!content) {
    fclose(file);
    return NULL;
  }

  fread(content, 1, file_size, file);
  content[file_size] = '\0';

  fclose(file);
  return content;
}

void write_file(const char *filename, const char *data) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    return;
  }
  fwrite(data, sizeof(char), strlen(data), file);
  fclose(file);
}

void append_file(const char *filename, const char *data) {
  FILE *file = fopen(filename, "ab");
  if (!file) {
    return;
  }
  fwrite(data, sizeof(char), strlen(data), file);
  fclose(file);
}

void free_file_content(char *content) { free(content); }
