#include "lexer.h"

#include <ctype.h>

char lexer_advance(lexer_t *lexer) {
  if (lexer->current_char != '\0') {
    lexer->pos++;
    lexer->current_char = lexer->next_char;
    if (lexer->pos + 1 < strlen(lexer->input)) {
      lexer->next_char = lexer->input[lexer->pos + 1];
    } else {
      lexer->next_char = '\0'; // end of input
    }
  }
  return lexer->current_char;
}

void skip_whitespace(lexer_t *lexer) {
  while (lexer->current_char != '\0' &&
         (lexer->current_char == ' ' || lexer->current_char == '\t' ||
          lexer->current_char == '\n' || lexer->current_char == '\r')) {
    lexer_advance(lexer);
  }
}

int read_int(lexer_t *lexer) {
  size_t start_pos = lexer->pos;
  while (lexer->current_char != '\0' && lexer->current_char >= '0' &&
         lexer->current_char <= '9') {
    lexer_advance(lexer);
  }
  size_t length = lexer->pos - start_pos;
  char *num_str = (char *)malloc(length + 1);
  strncpy(num_str, lexer->input + start_pos, length);
  num_str[length] = '\0';

  int value = atoi(num_str);
  free(num_str);

  return value;
}

char *read_identifier(lexer_t *lexer) {
  size_t start_pos = lexer->pos;
  while (lexer->current_char != '\0' &&
         (isalnum(lexer->current_char) || lexer->current_char == '_')) {
    lexer_advance(lexer);
  }
  size_t length = lexer->pos - start_pos;
  char *id_str = (char *)malloc(length + 1);
  strncpy(id_str, lexer->input + start_pos, length);
  id_str[length] = '\0';

  return id_str;
}

token_t lexer_next_token(lexer_t *lexer) {
  char current_char = lexer_advance(lexer);
  token_t token = {0};

  // skip whitespace
  skip_whitespace(lexer);

  switch (current_char) {
  case '+':
    token.type = TOKEN_PLUS;
    token.value = "+";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '-':
    token.type = TOKEN_MINUS;
    token.value = "-";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '*':
    token.type = TOKEN_MUL;
    token.value = "*";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '/':
    token.type = TOKEN_DIV;
    token.value = "/";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '(':
    token.type = TOKEN_LPAR;
    token.value = "(";
    token.val_type = VAL_TYPE_STRING;
    break;
  case ')':
    token.type = TOKEN_RPAR;
    token.value = ")";
    token.val_type = VAL_TYPE_STRING;
    break;
  case ',':
    token.type = TOKEN_COMMA;
    token.value = ",";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '=':
    token.type = TOKEN_EQUAL;
    token.value = "=";
    token.val_type = VAL_TYPE_STRING;
    break;
  case ';':
    token.type = TOKEN_SEMICOLON;
    token.value = ";";
    token.val_type = VAL_TYPE_STRING;
    break;
  case '\0':
    token.type = TOKEN_EOF;
    token.value = "EOF";
    token.val_type = VAL_TYPE_STRING;
    break;
  default:
    // Handle identifiers and numbers here
    // TODO: edge cases and errors
    if (isdigit(current_char)) {
      token.type = TOKEN_NUMBER;
      token.value = read_int(lexer);
      token.val_type = VAL_TYPE_INT;
    } else if (isalpha(current_char)) {
      token.type = TOKEN_IDENTIFIER;
      token.value = read_identifier(lexer);
      token.val_type = VAL_TYPE_STRING;
    } else {
      // Unknown character
      token.type = TOKEN_UNKNOWN;
      char *error = "";
      snprintf(error, 2, "%c", current_char);
      token.value = error;
      token.val_type = VAL_TYPE_STRING;
    }

    break;
  }
  return token;
}