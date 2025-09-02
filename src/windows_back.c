#ifdef _WIN32
#include "windows_back.h"
#include "parser.h"

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *windows_codegen(node_t *ast) {
  if (!ast)
    return NULL;
  return "data";
}

int windows_run_nasm(const char *filename) {
  if (!filename || !*filename)
    return 1;

  // make a copy so we can strip extension
  char base[1024];
  snprintf(base, sizeof(base), "%s", filename);

  char *dot = strrchr(base, '.');
  if (dot)
    *dot = '\0';

  char outobj[1060];
  snprintf(outobj, sizeof(outobj), "%s.o", base);

  printf("Compiling %s -> %s\n", filename, outobj);

  // spawn NASM: program, argv[0], argv[1]...
  return _spawnl(_P_WAIT,
                 ".\\tools\\nasm.exe", // program path
                 "nasm",               // argv[0]
                 "-fwin64",            // format
                 filename,             // input file
                 "-o", outobj,         // output file
                 NULL);
}

int windows_run_ld(const char *filename, const char **libs) {
  if (!filename || !*filename)
    return 1;

  char base[1024];
  snprintf(base, sizeof(base), "%s", filename);

  char *dot = strrchr(base, '.');
  if (dot)
    *dot = '\0';
  char outexe[1060];
  snprintf(outexe, sizeof(outexe), "%s.exe", base);

  printf("Linking %s -> %s with libs ", filename, outexe);
  if (libs) {
    for (const char **lib = libs; *lib != NULL; lib++) {
      printf("%s, ", *lib);
    }
  } else {
    printf("none");
  }
  printf("\n");

  // add libs to command line
  char libargs[4096] = "";
  if (libs) {
    for (const char **lib = libs; *lib != NULL; lib++) {
      char buf[256];
      snprintf(buf, sizeof(buf), "-l%s ", *lib);
      strncat(libargs, buf, sizeof(libargs) - strlen(libargs) - 1);
    }
  }

  return _spawnl(_P_WAIT,
                 ".\\tools\\ld.exe", // program path
                 "ld",               // argv[0]
                 "-e_start",         // entry point
                 "-o", outexe,       // output file
                 filename,           // input file
                 libargs,            // libraries
                 NULL);
}

int windows_compile_asm(const char *filename, const char **libs) {
  if (windows_run_nasm(filename) != 0) {
    fprintf(stderr, "Error: NASM failed\n");
    return 1;
  }

  char base[1024];
  snprintf(base, sizeof(base), "%s", filename);
  char *dot = strrchr(base, '.');
  if (dot)
    *dot = '\0';
  char outobj[1060];
  snprintf(outobj, sizeof(outobj), "%s.o", base);

  if (windows_run_ld(outobj, libs) != 0) {
    fprintf(stderr, "Error: LD failed\n");
    return 1;
  }

  return 0;
}
#endif // _WIN32