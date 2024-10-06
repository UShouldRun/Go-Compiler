#ifndef GOC_PARSER_PRIVATE_H
#define GOC_PARSER_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdboo.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "goc_error.h"
#include "goc_lexer.h"

// Terminology: AST <=> Abstract Syntax Tree

// Go (Modified) Type
enum type_go;

// AST Node Type
enum ast_node_type;

// =========# AST Nodes #============

// AST Node Single
struct ast_node_literal;
struct ast_node_result;

// AST Node Expression
struct ast_node_expr_member;
struct ast_node_expr_array;
struct ast_node_expr_call;
struct ast_node_expr_unary;
struct ast_node_expr_bin;

// AST Node Statements
struct ast_node_stmt_expr;
struct ast_node_stmt_assign;

struct ast_node_stmt_return;

struct ast_node_stmt_if;

struct ast_node_stmt_loop_do;
struct ast_node_stmt_loop_while;

union ast_node_stmt_init;
struct ast_node_stmt_loop_for;

struct ast_node_stmt;
struct ast_node_stmt_block;

// AST Node Declarations
struct ast_node_var_declare;
struct ast_node_func_declare;

// AST Node Objects
struct ast_node_var;
struct ast_node_func;
struct ast_node_method;
struct ast_node_union;
struct ast_node_enum;
struct ast_node_struct;
struct ast_node_interface;

// AST Node Super Objects
struct ast_node_file;
struct ast_node_package;
struct ast_node_program;

// ===============================

typedef enum type_go {
  GT_UNKNOW = 0,

  GT_VOID,

  GT_INT8,
  GT_INT16,
  GT_INT32,
  GT_INT64,

  GT_UINT8,
  GT_UINT16,
  GT_UINT32,
  GT_UINT64,

  GT_FLOAT,
  GT_DOUBLE,

  GT_CHAR,
  GT_STRING,

  GT_BOOL,

  GT_POINTER,
  GT_ARRAY,
  GT_STRUCT,
  GT_ENUM,
  GT_FUNCTION,
  GT_INTERFACE,
} TypeGo;

typedef enum ast_node_type {
  ast_undefined = 0,
  ast_token,
  ast_node_literal,
  ast_node_result,

  ast_node_expr_member,
  ast_node_expr_array,
  ast_node_expr_call,
  ast_node_expr_unary,
  ast_node_expr_bin,

  ast_node_stmt_expr,
  ast_node_stmt_assign,
  ast_node_stmt_return,
  ast_node_stmt_if,
  ast_node_stmt_loop_do,
  ast_node_stmt_loop_while,
  ast_node_stmt_loop_for,
  ast_node_stmt_block,

  ast_node_var_declare,
  ast_node_func_declare,

  ast_node_var,
  ast_node_func,
  ast_node_method,
  ast_node_union,
  ast_node_enum,
  ast_node_struct,
  ast_node_interface,

  ast_node_file,
  ast_node_package,
  ast_node_program
} AST_NodeType;

struct ast_node_result {
  TypeGo     type_go;
  union {
    int8_t   val_int8;
    int16_t  val_int16;
    int32_t  val_int32;
    int64_t  val_int64;

    uint8_t  val_uint8;
    uint16_t val_uint16;
    uint32_t val_uint32;
    uint64_t val_uint64;

    float    val_float;
    double   val_double;

    char     val_ch;
    char    *val_string;
  } result;
};

struct ast_node_literal {
  TypeGo type_go;
  Token  literal;
};

struct ast_node_expr_member {
  TypeGo type_go;
  Token  member;
};

struct ast_node_expr_array {
  TypeGo type_go;
  Token  array;
};

struct ast_node_expr_call {
  TypeGo type_go;
  Token  token;
  struct ast_node_func *func;
};

struct ast_node_expr_unary {
  TypeGo type_go;
  AST_NodeType ast_type_expr;
  Token unary_operator;
  union {
    Token token;
    struct ast_node_result      *result;
    struct ast_node_expr_unary  *expr_unary;
    struct ast_node_expr_bin    *expr_bin;
    struct ast_node_stmt_assign *stmt_assign;
  } operand;
};

struct ast_node_expr_bin {
  TypeGo        type_go_left,
                type_go_right;
  AST_NodeType  ast_type_expr_right,
                ast_type_expr_left;
  Token         expr_operator;
  union {
    Token token;
    struct ast_node_result      *result;
    struct ast_node_expr_unary  *expr_unary; 
    struct ast_node_expr_bin    *expr_bin;
    struct ast_node_stmt_assign *stmt_assign;
  } expr_bin_left;
  union {
    Token token;
    struct ast_node_result      *result;
    struct ast_node_expr_unary  *expr_unary;
    struct ast_node_expr_bin    *expr_bin;
    struct ast_node_stmt_assign *stmt_assign;
  } expr_bin_right;
};

struct ast_node_stmt_expr {
  TypeGo       type_go;
  AST_NodeType ast_type_stmt;
  union {
    Token token;
    struct ast_node_result      *result;
    struct ast_node_expr_bin    *expr_bin;
    struct ast_node_expr_unary  *expr_unary;
    struct ast_node_stmt_assign *stmt_assign;
  } expr;
};

struct ast_node_stmt_return {
  TypeGo       type_go;
  AST_NodeType ast_type_stmt;
  union {
    struct ast_node_stmt_expr   *stmt_expr;
    struct ast_node_stmt_assign *stmt_assign;
  } return;
};

struct ast_node_stmt_assign {
  TypeGo       type_go;
  AST_NodeType ast_type_stmt;
  union {
    struct ast_node_var       *var;
    struct ast_node_stmt_expr *stmt_expr;
  } assign;
};

struct ast_node_stmt_loop_while {
  struct ast_node_stmt_expr  *condition;
  struct ast_node_stmt_block *block;
};

struct ast_node_stmt_loop_do {
  struct ast_node_stmt_expr  *condition;
  struct ast_node_stmt_block *block;
};

union ast_node_stmt_init {
  struct ast_node_var_declare *declare;
  struct ast_node_stmt_assign *assign;
};

struct ast_node_stmt_loop_for {
  uint32_t s_init, s_update;
  AST_NodeType *ast_type_init;
  union  ast_node_init *init;
  struct ast_node_stmt_assign *update;
  struct ast_node_stmt_expr   *condition;
  struct ast_node_stmt_block  *block;
};

struct ast_node_stmt_if {
  bool has_else;
  uint32_t s_elseif;
  AST_NodeType ast_type_init;
  union  ast_node_init        *init;
  struct ast_node_stmt_expr   *condition;
  struct ast_node_stmt_block  *block;
  struct ast_node_stmt_if     *elseif;
  struct ast_node_stmt_block  *else;
};

struct ast_node_stmt {
  AST_NodeStmtType type;
  union {
    struct ast_node_stmt_expr       *stmt_exprs;
    struct ast_node_stmt_assign     *stmt_assigns;
    struct ast_node_stmt_return     *stmt_return;
    struct ast_node_stmt_if         *stmt_if;
    struct ast_node_stmt_loop_while *stmt_loop_while;
    struct ast_node_stmt_loop_for   *stmt_loop_for;
    struct ast_node_stmt_loop_do    *stmt_loop_do;
    struct ast_node_stmt_block      *stmt_block;
  } stmt;
};

struct ast_node_stmt_block {
  uint32_t s_node_stmts;
  struct ast_node_stmt *stmts;
};

union ast_node_type_ident {
  TypeGo type_go;
  Token  type_custom;
};

struct ast_node_var_declare {
  Token  token;
  bool   is_type_go; 
  union ast_node_type_ident type_var;
};

struct ast_node_var {
  struct ast_node_var_declare *declare;
  struct ast_node_stmt_expr   *value;
};

struct ast_node_func_declare {
  Token    func;
  uint32_t s_args,
           s_return; 
  bool    *is_type_go;
  struct ast_node_var        *args;
  union  ast_node_type_ident *return;
};

struct ast_node_func {
  struct ast_node_func_declare *declare;
  struct ast_node_stmt_block   *block;
};

struct ast_node_interface {
  TypeGo   type_go;
  Token    interface;
  uint32_t s_methods;
  struct ast_node_func_declare *methods;
};

struct ast_node_union {
  uint32_t s_types;
  union ast_node_type_ident *types;
};

struct ast_node_enum {
  bool     iota;
  uint32_t s_values,
           s_assign;
  Token   *values;
  Token   *assign;
};

struct ast_node_method {
  Token struct;
  struct ast_node_func *func;
};

struct ast_node_struct {
  uint32_t s_types;
  union ast_node_type_ident *types;
};

struct ast_node_import {
  uint32_t s_imports;
  const char *imports;
};

union ast_node_file_block {
  struct ast_node_import;

  struct ast_node_var;
  struct ast_node_var_declare;

  struct ast_node_func;
  struct ast_node_func_declare;

  struct ast_node_enum;
  struct ast_node_union;
  struct ast_node_struct;
  struct ast_node_method;
};

struct ast_node_file {
  uint32_t s_file_blocks;
  union ast_node_file_block *file_blocks;
};

struct ast_node_package {
  Token    package;
  uint32_t s_files;
  struct ast_node_file *files;
};

struct ast_node_program {
  uint32_t s_packages;
  struct ast_node_package *packages;
  struct ast_node_func    *main;
};

static void goc_parser_parse_file(AST_NodeProgram program, TokenArray array, size_t *word_ptr, bool main_file);

static void goc_parser_print_error(
  void goc_error_func(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word),
  Token token
);
static void   _goc_parser_find_package_dependencies();

static void  *_goc_parser_ast_node_create(AST_NodeType type, size_t s_nodes);
static size_t _goc_parser_ast_node_size(AST_NodeType type);

#endif // !GOC_PARSER_PRIVATE_H
