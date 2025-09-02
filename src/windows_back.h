#ifdef _WIN32
#pragma once

#include "parser.h"

char *windows_codegen(node_t *ast);

int windows_run_nasm(const char *filename);

int windows_run_ld(const char *filename, const char **libs);

int windows_compile_asm(const char *filename, const char **libs);
#endif // _WIN32