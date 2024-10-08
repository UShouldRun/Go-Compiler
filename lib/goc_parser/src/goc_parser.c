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

static void _goc_parser_find_package_dependencies() {
  return;
}

// AST Node Objects
static AST_NodeImport _goc_parser_ast_node_import_create(uint32_t s_imports, const char *imports);
static AST_NodeEnum _goc_parser_ast_node_enum_create(
  Token enum, bool iota, uint32_t s_values, uint32_t s_assign, TokenArray values, TokenArray assign
);
static AST_NodeUnion _goc_parser_ast_node_union_create(Token union, uint32_t s_types, AST_NodeTypeIdent types, TokenArray values);
static AST_NodeStruct _goc_parser_ast_node_struct_create(Token struct, AST_NodeTypeIdent types, uint32_t s_types); 
static AST_NodeMethod _goc_parser_ast_node_method_create(AST_NodeStruct struct, AST_NodeFunc func);
static AST_NodeVar _goc_parser_ast_node_var_create(Token token, bool is_type_go, AST_NodeTypeIdent type_var, AST_NodeExpr value);
static AST_NodeFunc _goc_parser_ast_node_func_create(
  AST_NodeFuncDeclare declare, AST_NodeStmtBlock block, AST_NodeStmtReturn return
);
static AST_NodeFuncDeclare _goc_parser_ast_node_func_declare_create(
  Token func, uint32_t s_args, uint32_t s_return, bool *is_type_go, AST_NodeVar args, AST_NodeTypeIdent type_return
);

// AST Node Statements
static AST_NodeStmtBlock _goc_parser_ast_node_stmt_block_create(AST_NodeType *type_stmts, void *stmts, uint64_t s_stmts);
static AST_NodeStmt _goc_parser_ast_node_stmt_create(AST_NodeType type_stmt, void *stmt);
static AST_NodeStmtFor _goc_parser_ast_node_stmt_loop_for_create(
  uint32_t s_init, uint64_t s_update, AST_NodeVar init, AST_NodeExpr condition, AST_NodeStmtAssign update, AST_NodeStmtBlock block
);
static AST_NodeStmtWhile _goc_parser_ast_node_stmt_loop_while_create(
  bool loop_while, struct ast_node_expr *condition, struct ast_node_stmt_block *block
);
static AST_NodeStmtIf _goc_parser_ast_node_stmt_if_create(
  bool has_else, uint32_t s_elseif, AST_NodeType type_init, union ast_node_init *init, struct ast_node_expr *condition,
  struct ast_node_stmt_block *if_block, struct ast_node_stmt_if *elseif, struct ast_node_stmt_block *else_block
);
static AST_NodeStmtReturn _goc_parser_ast_node_stmt_return_create(AST_NodeType type_stmt, void *return);
static AST_NodeStmtAssign _goc_parser_ast_node_stmt_assign_create(struct ast_node_var *var, struct ast_node_stmt_expr *assign);

// AST Node Expression
static AST_NodeExpr _goc_parser_ast_node_expr_create(AST_NodeType type_node, void *expr);
static AST_NodeExprBin _goc_parser_ast_node_expr_bin_create(
  AST_NodeType type_expr_left, AST_NodeType type_expr_right, Token bin_op, void *expr_left, void *expr_right
);
static AST_NodeExprUnary _goc_parser_ast_node_expr_unary_create(AST_NodeType type_expr, Token unary_op, void *operand);
static AST_NodeExprMember _goc_parser_ast_node_expr_member_create(AST_NodeType type_ds, uint64_t offset, void *data_structure);

// AST Node Literal
static AST_NodeLiteral _goc_parser_ast_node_literal(TypeGo type_go, void *literal) {
  goc_error_assert(nullptr, literal != NULL);
  AST_NodeLiteral literal = (AST_NodeLiteral)_goc_parser_ast_node_create(ast_node_literal, 1);
  literal->type_go = type_go;
  switch (type_go) {
    case TG_BOOL:
      literal->val_bool  = *literal;

    case TG_INT8:
      literal->val_int8  = *literal;
    case TG_INT16:
      literal->val_int16 = *literal;
    case TG_INT32: case TG_INT:
      literal->val_int32 = *literal;
    case TG_INT64:
      literal->val_int64 = *literal;

    case TG_UINT8:
      literal->val_uint8  = *literal;
    case TG_UINT16:
      literal->val_uint16 = *literal;
    case TG_UINT32: case TG_UINT:
      literal->val_uint32 = *literal;
    case TG_UINT64:
      literal->val_uint64 = *literal;

    case TG_FLOAT:
      literal->val_float  = *literal;
    case TG_DOUBLE:
      literal->val_double = *literal;

    case TG_CHAR:
      literal->val_char   = *literal;
    case TG_STRING:
      literal->val_string = (char *)literal;
  }
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
