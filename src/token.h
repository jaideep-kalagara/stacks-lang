#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum token_type {
  TOKEN_EOF,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_LPAR,
  TOKEN_RPAR,
  TOKEN_COMMA,
  TOKEN_EQUAL,
  TOKEN_SEMICOLON,
  TOKEN_IDENTIFIER,
  TOKEN_KEYWORD,
  TOKEN_UNKNOWN,
};

enum identifier_type {
  IDENTIFIER_VARIABLE,
  IDENTIFIER_FUNCTION,
};

enum keyword_type {
  KEYWORD_EXIT,
};

enum val_type {
  VAL_TYPE_INT,
  VAL_TYPE_STRING,
  VAL_TYPE_IDENTIFIER,
  VAL_TYPE_KEYWORD,
};

typedef enum token_type token_type_t;
typedef enum identifier_type identifier_type_t;
typedef enum keyword_type keyword_type_t;
typedef enum val_type val_type_t;

struct token {
  token_type_t type;
  void *value;         /* could be char* or a int or even a identifier_type_t*/
  val_type_t val_type; /* tells us what type the value is  */
};

typedef struct token token_t;

// makes a new token and returns it as a pointer, dont forget to free
token_t *new_token(token_type_t type, void *value, val_type_t val_type);
// frees a token and its value if its a string
void free_token(token_t *token);

// prints a token to stdout
void print_token(token_t *token);