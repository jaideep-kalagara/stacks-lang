#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "token.h"

struct lexer {
  const char *input;
  size_t pos;
  char current_char;
  char next_char;
};

typedef struct lexer lexer_t;

// initializes a lexer with the given input string
void lexer_init(lexer_t *lx, const char *input);

// advances the lexer by one character making current_char the next_char and
// next_char the next character in the input
// returns the new current_char or '\0' if at end of input
char lexer_advance(lexer_t *lexer);

// returns the next token in the input
token_t *lexer_next_token(lexer_t *lexer);