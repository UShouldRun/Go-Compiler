#include "goc_parser.h"
#include "goc_parser_private.h"

// =======================================================# PUBLIC #==================================================================

AST_NodeProgram goc_parser(FILE *file, TokenArray array) {
  if (array == NULL)
    return NULL;

  size_t word_ptr = 0;

  AST_NodeProgram program = (AST_NodeProgram)_goc_parser_ast_node_create(ast_node_program, 1);
  goc_error_assert(goc_error_nullptr, program != NULL);

  TokenType tt_package = goc_lexer_token_get_type(goc_lexer_token_array_at(array, word_ptr)),
            tt_ident   = goc_lexer_token_get_type(goc_lexer_token_array_at(array, word_ptr + 1));
  bool tt_main = tt_ident == TT_IDENT ? 
       (strcmp(goc_lexer_token_get_value_text(goc_lexer_token_array_at(array, word_ptr + 1)), KEYWORD_MAIN) == 0) : false;
  if (tt_package != TT_PACKAGE || !tt_main) {
    Token not_main = goc_lexer_token_array_at(array, word_ptr + 1);
    goc_parser_print_error(goc_error_parser_print_package_main_not_found, not_main);
  }

  goc_parser_parse_file(program, array, &word_ptr, true);

  return program;
}

// =======================================================# PRIVATE #==================================================================

static void goc_parser_parse_file(AST_NodeProgram program, TokenArray array, size_t *word_ptr, bool main_file) {
  
}

static void goc_parser_print_error(
  void goc_error_func(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word),
  Token token
) {
  goc_error_func(
    file,
    goc_lexer_token_get_pos_rel(not_main),
    goc_lexer_token_get_pos_line(not_main),
    goc_lexer_token_get_pos_abs(not_main),
    goc_lexer_token_get_pos_s_word(not_main)
  );
}

static void _goc_parser_find_package_dependencies() {
  return;
}

static void *_goc_parser_ast_node_create(AST_NodeType type, size_t s_nodes) {
  return malloc(s_nodes * _goc_parser_ast_node_size(type));
}

static size_t _goc_parser_ast_node_size(AST_NodeType type) {
  switch (type) {
    case ast_node_literal:         return sizeof(struct ast_node_literal);
    case ast_node_result:          return sizeof(struct ast_node_result);

    case ast_node_expr_member:     return sizeof(struct ast_node_expr_member);
    case ast_node_expr_array:      return sizeof(struct ast_node_expr_array);
    case ast_node_expr_call:       return sizeof(struct ast_node_expr_call);
    case ast_node_expr_unary:      return sizeof(struct ast_node_expr_unary);
    case ast_node_expr_bin:        return sizeof(struct ast_node_expr_bin);

    case ast_node_stmt_expr:       return sizeof(struct ast_node_stmt_expr);
    case ast_node_stmt_assign:     return sizeof(struct ast_node_stmt_assign);
    case ast_node_stmt_return:     return sizeof(struct ast_node_stmt_return);
    case ast_node_stmt_if:         return sizeof(struct ast_node_stmt_if);
    case ast_node_stmt_loop_do:    return sizeof(struct ast_node_stmt_loop_do);
    case ast_node_stmt_loop_while: return sizeof(struct ast_node_stmt_loop_while);
    case ast_node_stmt_loop_for:   return sizeof(struct ast_node_stmt_loop_for);
    case ast_node_stmt_block:      return sizeof(struct ast_node_stmt_block);

    case ast_node_var_declare:     return sizeof(struct ast_node_var_declare);
    case ast_node_func_declare:    return sizeof(struct ast_node_func_declare);
    case ast_node_var:             return sizeof(struct ast_node_var);
    case ast_node_func:            return sizeof(struct ast_node_func);
    case ast_node_method:          return sizeof(struct ast_node_method);
    case ast_node_union:           return sizeof(struct ast_node_union);
    case ast_node_enum:            return sizeof(struct ast_node_enum);
    case ast_node_struct:          return sizeof(struct ast_node_struct);
    case ast_node_interface:       return sizeof(struct ast_node_interface);

    case ast_node_file:            return sizeof(struct ast_node_file);
    case ast_node_package:         return sizeof(struct ast_node_package);
    case ast_node_program:         return sizeof(struct ast_node_program);

    default:                       return 0;
  }
}
