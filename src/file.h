#pragma once

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename);
void write_file(const char *filename, const char *data);
void append_file(const char *filename, const char *data);
void free_file_content(char *content);