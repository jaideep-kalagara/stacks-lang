#include "lexer.h"
#include "token.h"
#include "windows_back.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  // TEST: load asm file and compile it
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

  // TEST: token parsing
  char *input = "exit 42;";
  lexer_t lexer = {input, 0, input[0], input[1]};

  while (token_t token = lexer_advance(&lexer); token.type != TOKEN_EOF) {
    print_token(&token);
  }

  return 0;
}
