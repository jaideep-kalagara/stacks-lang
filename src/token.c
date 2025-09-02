#include "token.h"
#include <stdbool.h>
#include <stdlib.h>

token_t *new_token(token_type_t type, void *value, val_type_t val_type) {
  token_t *token = malloc(sizeof(token_t));
  token->type = type;
  token->value = value;
  token->val_type = val_type;

  return token;
}

void free_token(token_t *token) {
  if (!token)
    return;
  if (token->value && token->val_type == VAL_TYPE_STRING)
    free(token->value);
  free(token);
}

static const char *token_type_names[] = {
    "TOKEN_EOF",      "TOKEN_NUMBER",    "TOKEN_PLUS",       "TOKEN_MINUS",
    "TOKEN_ASTERISK", "TOKEN_SLASH",     "TOKEN_LPAREN",     "TOKEN_RPAREN",
    "TOKEN_COMMA",    "TOKEN_SEMICOLON", "TOKEN_IDENTIFIER", "TOKEN_KEYWORD",
    "TOKEN_UNKNOWN"};

void print_token(token_t *token) {
  if (!token)
    return;
  if (token->val_type == VAL_TYPE_STRING) {
    printf("token: %s value: \"%s\"\n", token_type_names[token->type],
           (char *)token->value);
    return;
  } else if (token->val_type == VAL_TYPE_IDENTIFIER) {
    printf("token: %s value: %d\n", token_type_names[token->type],
           *(int *)token->value);
    return;
  } else if (token->val_type == VAL_TYPE_KEYWORD) {
    printf("token: %s value: %d\n", token_type_names[token->type],
           (int)(intptr_t)token->value);
    return;
  } else if (token->val_type == VAL_TYPE_INT) {
    printf("token: %s value: %d\n", token_type_names[token->type],
           (int)(intptr_t)token->value);
    return;
  }
}