#include "lexer.h"
#include "token.h"
#include "windows_back.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // TEST: load asm file and compile it for windows
  // if (argc != 2) {
  //   printf("Usage: %s <filename>\n", argv[0]);
  //   return 1;
  // }
  // const char *libs[] = {"kernel32", "user32", NULL};

  // if (windows_compile_asm(argv[1], libs)) {
  //   printf("Failed to compile %s\n", argv[1]);
  //   return 1;
  // }

  // TEST: token creation and deletion
  // token_t *t1 = new_token(TOKEN_NUMBER, (void *)(intptr_t)42, VAL_TYPE_INT);
  // token_t *t2 =
  //     new_token(TOKEN_STRING, strdup("Hello, World!"), VAL_TYPE_STRING);

  // printf("Created tokens:\n");
  // print_token(t1);
  // print_token(t2);

  // free_token(t1);
  // free_token(t2);

  // TEST: lexing
  // const char *input = "exit 0;";
  // lexer_t lx;
  // lexer_init(&lx, input);

  // printf("Lexing input: %s\n", input);

  // for (;;) {
  //   token_t *t = lexer_next_token(&lx);
  //   if (t->type == TOKEN_EOF) {
  //     free_token(t);
  //     break;
  //   }
  //   print_token(t);
  //   free_token(t);
  // }

  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (!file) {
    printf("Failed to open file %s\n", argv[1]);
    return 1;
  }

  // get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *file_content = (char *)malloc(file_size + 1);

  // read file content into buffer
  fread(file_content, 1, file_size, file);
  file_content[file_size] = '\0';

  fclose(file);

  // Lexing the file content
  lexer_t lx;
  lexer_init(&lx, file_content);

  for (;;) {
    token_t *t = lexer_next_token(&lx);
    if (t->type == TOKEN_EOF) {
      free_token(t);
      break;
    }
    print_token(t);
    free_token(t);
  }

  free(file_content);
  return 0;
}
