#pragma once
#include "lexer.h"
#include "token.h"
#include <stddef.h> // (optional) for size_t
#include <stdio.h>
#include <stdlib.h>

// ----- vector of node pointers -----
typedef struct {
  struct node **items;
  size_t len;
  size_t cap;
} node_vec_t;

// ----- AST node -----
typedef enum {
  NK_PROGRAM,
  NK_NUMBER,
  NK_IDENTIFIER,
  NK_BINARY,
  NK_ASSIGN,
  NK_EXIT
} node_type_t;

typedef struct node {
  node_type_t type;
  union {
    int number;       // NK_NUMBER
    char *identifier; // NK_IDENTIFIER
    struct {          // NK_BINARY
      token_type_t op;
      struct node *lhs, *rhs;
    } binary;
    struct { // NK_ASSIGN
      char *name;
      struct node *expr;
    } assign;
    struct { // NK_EXIT
      struct node *expr;
    } exit_;
    struct { // NK_PROGRAM
      node_vec_t body;
    } program;
  } data;
} node_t;

// ----- small helpers (mark inline to avoid multiple definitions from header)
// -----
static inline void vec_grow(node_vec_t *v) {
  size_t ncap = v->cap ? v->cap * 2 : 8;
  v->items = (node_t **)realloc(v->items, ncap * sizeof(node_t *));
  v->cap = ncap;
}

static inline node_t *make_program(void) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_PROGRAM;
  return n;
}

static inline void program_push(node_t *prog, node_t *stmt) {
  if (!prog || prog->type != NK_PROGRAM)
    return;
  node_vec_t *b = &prog->data.program.body;
  if (b->len == b->cap)
    vec_grow(b); // <-- fixed (len == cap)
  b->items[b->len++] = stmt;
}

static inline node_t *make_num(int value) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_NUMBER;
  n->data.number = value;
  return n;
}

static inline node_t *make_id(char *name) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_IDENTIFIER;
  n->data.identifier = name;
  return n;
}

static inline node_t *make_binary(token_type_t op, node_t *lhs, node_t *rhs) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_BINARY;
  n->data.binary.op = op;
  n->data.binary.lhs = lhs;
  n->data.binary.rhs = rhs;
  return n;
}

static inline node_t *make_assign(char *name, node_t *expr) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_ASSIGN;
  n->data.assign.name = name;
  n->data.assign.expr = expr;
  return n;
}

static inline node_t *make_exit(node_t *expr) {
  node_t *n = (node_t *)calloc(1, sizeof *n);
  n->type = NK_EXIT;
  n->data.exit_.expr = expr; // renamed member to exit_ to avoid confusion
  return n;
}

static inline void free_node(node_t *n) {
  if (!n)
    return;
  switch (n->type) {
  case NK_PROGRAM:
    for (size_t i = 0; i < n->data.program.body.len; ++i)
      free_node(n->data.program.body.items[i]);
    free(n->data.program.body.items);
    break;
  case NK_NUMBER:
    break;
  case NK_IDENTIFIER:
    free(n->data.identifier);
    break;
  case NK_BINARY:
    free_node(n->data.binary.lhs);
    free_node(n->data.binary.rhs);
    break;
  case NK_ASSIGN:
    free(n->data.assign.name);
    free_node(n->data.assign.expr);
    break;
  case NK_EXIT:
    free_node(n->data.exit_.expr);
    break;
  }
  free(n);
}

static inline void print_ast(node_t *n, int d) {
  for (int i = 0; i < d; ++i)
    putchar(' ');
  switch (n->type) {
  case NK_PROGRAM:
    printf("Program\n");
    for (size_t i = 0; i < n->data.program.body.len; ++i)
      print_ast(n->data.program.body.items[i], d + 2);
    break;
  case NK_NUMBER:
    printf("Number(%d)\n", n->data.number);
    break;
  case NK_IDENTIFIER:
    printf("Ident(%s)\n", n->data.identifier);
    break;
  case NK_BINARY:
    printf("Bin(%s)\n", n->data.binary.op == TOKEN_PLUS    ? "+"
                        : n->data.binary.op == TOKEN_MINUS ? "-"
                        : n->data.binary.op == TOKEN_MUL   ? "*"
                        : n->data.binary.op == TOKEN_DIV   ? "/"
                                                           : "?");
    print_ast(n->data.binary.lhs, d + 2);
    print_ast(n->data.binary.rhs, d + 2);
    break;
  case NK_ASSIGN:
    printf("Assign(%s)\n", n->data.assign.name);
    print_ast(n->data.assign.expr, d + 2);
    break;
  case NK_EXIT:
    printf("Exit\n");
    print_ast(n->data.exit_.expr, d + 2);
    break;
  }
}

// ----- parser bits (don’t put 'static' on declarations in a header) -----
typedef struct {
  lexer_t lexer;
  token_t *current_token;
  token_t *next_token;
} parser_t;

void parser_init(parser_t *p, const char *src);
void parser_destroy(parser_t *p);
node_t *parse_statement(parser_t *p);
