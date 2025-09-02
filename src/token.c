#include "token.h"
#include <stdlib.h>

token_t *new_token(token_type_t type, void *value, val_type_t val_type) {
  token_t *t = (token_t *)malloc(sizeof(token_t));
  t->type = type;
  t->val_type = val_type;
  t->owns_string = false;

  switch (val_type) {
  case VAL_TYPE_INT:
    t->v.i = (int)(intptr_t)value;
    break;

  case VAL_TYPE_KEYWORD:
    t->v.keyword = (keyword_type_t)(intptr_t)value;
    break;

  case VAL_TYPE_IDENTIFIER:
    // store the enum value (caller passes it via intptr_t)
    t->v.ident = (identifier_type_t)(intptr_t)value;
    break;

  case VAL_TYPE_STRING:
    if (value == NULL) {
      t->v.s = NULL;
      t->owns_string = false;
    } else {
      if (type == TOKEN_IDENTIFIER) {
        t->v.s = (char *)value; // already malloc'ed by caller (lexer)
        t->owns_string = true;
      } else {
        t->v.s = strdup((const char *)value);
        t->owns_string = (t->v.s != NULL);
      }
    }
    break;
  }
  return t;
}

void free_token(token_t *t) {
  if (!t)
    return;
  if (t->val_type == VAL_TYPE_STRING && t->owns_string && t->v.s) {
    free(t->v.s);
  }
  free(t);
}

void print_token(token_t *t) {
  if (!t)
    return;

  switch (t->val_type) {
  case VAL_TYPE_INT:
    printf("token: %s value: %d\n", token_type_names[t->type], t->v.i);
    break;

  case VAL_TYPE_KEYWORD:
    printf("token: %s value: %s\n", token_type_names[t->type],
           keyword_type_names[(int)t->v.keyword]);
    break;

  case VAL_TYPE_IDENTIFIER:
    printf("token: %s value: %d\n", token_type_names[t->type], (int)t->v.ident);
    break;

  case VAL_TYPE_STRING:
  default:
    printf("token: %s value: \"%s\"\n", token_type_names[t->type],
           t->v.s ? t->v.s : "");
    break;
  }
}
