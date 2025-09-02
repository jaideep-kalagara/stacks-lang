#pragma once

#include <stdbool.h>
#include <stdint.h> // for intptr_t casts
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

static const char *token_type_names[] = {
    "TOKEN_EOF",        "TOKEN_NUMBER",  "TOKEN_STRING", "TOKEN_PLUS",
    "TOKEN_MINUS",      "TOKEN_MUL",     "TOKEN_DIV",    "TOKEN_LPAR",
    "TOKEN_RPAR",       "TOKEN_COMMA",   "TOKEN_EQUAL",  "TOKEN_SEMICOLON",
    "TOKEN_IDENTIFIER", "TOKEN_KEYWORD", "TOKEN_UNKNOWN"};

enum identifier_type {
  IDENTIFIER_VARIABLE,
  IDENTIFIER_FUNCTION,
};

enum keyword_type {
  KEYWORD_EXIT,
};

static const char *keyword_type_names[] = {
    "KEYWORD_EXIT",
};

enum val_type {
  VAL_TYPE_INT,        // v.i
  VAL_TYPE_STRING,     // v.s (heap if owns_string = true)
  VAL_TYPE_IDENTIFIER, // v.ident (enum)
  VAL_TYPE_KEYWORD,    // v.keyword (enum)
};

typedef enum token_type token_type_t;
typedef enum identifier_type identifier_type_t;
typedef enum keyword_type keyword_type_t;
typedef enum val_type val_type_t;

typedef struct token {
  token_type_t type;
  val_type_t val_type;

  union {
    int i;                   // VAL_TYPE_INT
    char *s;                 // VAL_TYPE_STRING
    identifier_type_t ident; // VAL_TYPE_IDENTIFIER
    keyword_type_t keyword;  // VAL_TYPE_KEYWORD
  } v;

  // Only meaningful for VAL_TYPE_STRING. If true, free(v.s) in free_token.
  bool owns_string;
} token_t;

// API stays the same
token_t *new_token(token_type_t type, void *value, val_type_t val_type);
void free_token(token_t *token);
void print_token(token_t *token);
