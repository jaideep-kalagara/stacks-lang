#include "parser.h"

void parser_init(parser_t *p, const char *src) {
  lexer_init(&p->lexer, src);
  p->current_token = lexer_next_token(&p->lexer);
  p->next_token = lexer_next_token(&p->lexer);
}

void parser_destroy(parser_t *p) {
  if (p->current_token)
    free_token(p->current_token);
  if (p->next_token)
    free_token(p->next_token);
}

static void parser_advance(parser_t *p) {
  if (p->current_token)
    free_token(p->current_token);
  p->current_token = p->next_token;
  p->next_token = lexer_next_token(&p->lexer);
}

static int parser_accept(parser_t *p, token_type_t type) {
  if (p->current_token->type == type) {
    parser_advance(p);
    return 1;
  }
  return 0;
}

static void parser_expect(parser_t *p, token_type_t type, const char *msg) {
  if (p->current_token->type != type) {
    fprintf(stderr, "Parse error: expected %s, got token type %d\n", msg,
            p->current_token->type);
    exit(1);
  }
  parser_advance(p);
}

static node_t *parse_expression(parser_t *p);

static node_t *parse_factor(parser_t *p) {
  if (p->current_token->type == TOKEN_NUMBER) {
    int val = p->current_token->v.i;
    parser_advance(p);
    return make_num(val);
  }
  if (p->current_token->type == TOKEN_IDENTIFIER) {
    char *name = p->current_token->v.s;
    node_t *n = make_id(name);
    parser_advance(p);
    return n;
  }
  if (parser_accept(p, TOKEN_LPAR)) {
    node_t *expr = parse_expression(p);
    parser_expect(p, TOKEN_RPAR, "')'");
    return expr;
  }
  fprintf(stderr, "Parse error: unexpected token type %d in factor\n",
          p->current_token->type);
  exit(1);
}

static node_t *parse_term(parser_t *p) {
  node_t *lhs = parse_factor(p);
  while (p->current_token->type == TOKEN_MUL ||
         p->current_token->type == TOKEN_DIV) {
    token_type_t op = p->current_token->type;
    parser_advance(p);
    node_t *rhs = parse_factor(p);
    lhs = make_binary(op, lhs, rhs);
  }
  return lhs;
}

static node_t *parse_expression(parser_t *p) {
  node_t *lhs = parse_term(p);
  while (p->current_token->type == TOKEN_PLUS ||
         p->current_token->type == TOKEN_MINUS) {
    token_type_t op = p->current_token->type;
    parser_advance(p);
    node_t *rhs = parse_term(p);
    lhs = make_binary(op, lhs, rhs);
  }
  return lhs;
}

node_t *parse_statement(parser_t *p) {
  if (p->current_token->type == TOKEN_KEYWORD &&
      p->current_token->v.keyword == KEYWORD_EXIT) {
    parser_advance(p);
    node_t *expr = parse_expression(p);
    parser_expect(p, TOKEN_SEMICOLON, "';'");
    return make_exit(expr);
  }
  if (p->current_token->type == TOKEN_IDENTIFIER &&
      p->next_token->type == TOKEN_EQUAL) {
    char *name = p->current_token->v.s;
    parser_advance(p); // consume identifier
    parser_advance(p); // consume '='
    node_t *expr = parse_expression(p);
    parser_expect(p, TOKEN_SEMICOLON, "';'");
    return make_assign(name, expr);
  }

  node_t *expr = parse_expression(p);
  parser_expect(p, TOKEN_SEMICOLON, "';'");
  return expr;
}