#include "file.h"
#include "lexer.h"
#include "token.h"
#include "windows_back.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  // Reading the file
  char *file_content = read_file(argv[1]);
  if (!file_content) {
    fprintf(stderr, "Error reading file: %s\n", argv[1]);
    return 1;
  }
  size_t file_size = strlen(file_content);

  // Lexing the file content
  lexer_t lx;
  lexer_init(&lx, file_content);

  token_t **tokens = (token_t **)malloc(file_size * sizeof(token_t *));
  size_t token_count = 0;

  for (;;) {
    token_t *t = lexer_next_token(&lx);
    if (t->type == TOKEN_EOF) {
      tokens[token_count++] = t;
      break;
    }
    tokens[token_count++] = t;
  }

  goto cleanup;

  // cleanup
cleanup:
  for (size_t i = 0; i < token_count; i++) {
    print_token(tokens[i]);
    free_token(tokens[i]);
  }
  free_file_content(file_content);
  free(tokens);

  return 0;
}
