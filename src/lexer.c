#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void lexer_init(lexer_t *lx, const char *input) {
  lx->input = input;
  lx->pos = 0;
  lx->current_char = input[0];
  lx->next_char = input[0] != '\0' ? input[1] : '\0';
}

char lexer_advance(lexer_t *lx) {
  if (lx->current_char != '\0') {
    lx->pos++;
    lx->current_char = lx->input[lx->pos];
    lx->next_char = lx->input[lx->pos + 1]; // safe: reads '\0' at end
  }
  return lx->current_char;
}

static void skip_whitespace(lexer_t *lx) {
  while (lx->current_char != '\0' && isspace((unsigned char)lx->current_char)) {
    lexer_advance(lx);
  }
}

static int read_int(lexer_t *lx) {
  int value = 0;
  while (lx->current_char >= '0' && lx->current_char <= '9') {
    value = value * 10 + (lx->current_char - '0');
    lexer_advance(lx);
  }
  return value;
}

static char *read_identifier(lexer_t *lx) {
  size_t start = lx->pos;
  while (
      lx->current_char != '\0' &&
      (isalnum((unsigned char)lx->current_char) || lx->current_char == '_')) {
    lexer_advance(lx);
  }
  size_t len = lx->pos - start;
  char *id = (char *)malloc(len + 1);
  if (!id)
    return NULL;
  memcpy(id, lx->input + start, len);
  id[len] = '\0';
  return id;
}

token_t *lexer_next_token(lexer_t *lx) {
  skip_whitespace(lx);

  char c = lx->current_char;
  if (c == '\0') {
    return new_token(TOKEN_EOF, "EOF", VAL_TYPE_STRING);
  }

  // Single-char tokens
  switch (c) {
  case '+':
    lexer_advance(lx);
    return new_token(TOKEN_PLUS, "+", VAL_TYPE_STRING);
  case '-':
    lexer_advance(lx);
    return new_token(TOKEN_MINUS, "-", VAL_TYPE_STRING);
  case '*':
    lexer_advance(lx);
    return new_token(TOKEN_MUL, "*", VAL_TYPE_STRING);
  case '/':
    lexer_advance(lx);
    return new_token(TOKEN_DIV, "/", VAL_TYPE_STRING);
  case '(':
    lexer_advance(lx);
    return new_token(TOKEN_LPAR, "(", VAL_TYPE_STRING);
  case ')':
    lexer_advance(lx);
    return new_token(TOKEN_RPAR, ")", VAL_TYPE_STRING);
  case ',':
    lexer_advance(lx);
    return new_token(TOKEN_COMMA, ",", VAL_TYPE_STRING);
  case '=':
    lexer_advance(lx);
    return new_token(TOKEN_EQUAL, "=", VAL_TYPE_STRING);
  case ';':
    lexer_advance(lx);
    return new_token(TOKEN_SEMICOLON, ";", VAL_TYPE_STRING);
  default:
    break;
  }

  if (isdigit((unsigned char)c)) {
    int val = read_int(lx);
    return new_token(TOKEN_NUMBER, (void *)(intptr_t)val, VAL_TYPE_INT);
  }

  if (isalpha((unsigned char)c) || c == '_') {
    char *id = read_identifier(lx);

    if (!id)
      return new_token(TOKEN_UNKNOWN, "alloc", VAL_TYPE_STRING);

    if (strcmp(id, "exit") == 0) {
      free(id); // avoid leaking the temporary identifier buffer
      return new_token(TOKEN_KEYWORD, (void *)(intptr_t)KEYWORD_EXIT,
                       VAL_TYPE_KEYWORD);
    }
    return new_token(TOKEN_IDENTIFIER, id, VAL_TYPE_STRING);
  }

  // Unknown: consume one char to make progress
  lexer_advance(lx);
  return new_token(TOKEN_UNKNOWN, "Unknown", VAL_TYPE_STRING);
}
