#include "file.h"
#include "lexer.h"
#include "parser.h"
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

  // Initialize parser
  parser_t parser;
  parser_init(&parser, file_content);
  node_t *root = make_program();

  while (parser.current_token->type != TOKEN_EOF) {
    node_t *stmt = parse_statement(&parser);
    program_push(root, stmt);
  }

  print_ast(root, 0);

  // char *code = windows_codegen(root);
  // write_file("temp.asm", code);

  goto cleanup;

  // cleanup
cleanup:
  parser_destroy(&parser);
  free_node(root);
  free_file_content(file_content);

  return 0;
}
