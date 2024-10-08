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
#include "goc_parser.h"

// Terminology: AST <=> Abstract Syntax Tree

// Go (Modified) Type
typedef enum type_go {
  TG_UNKNOW = 0,
  TG_VOID, TG_BOOL,
  TG_INT8, TG_INT16, TG_INT32, TG_INT64,
  TG_UINT8, TG_UINT16, TG_UINT32, TG_UINT64,
  TG_FLOAT, TG_DOUBLE,
  TG_CHAR, TG_STRING,
  TG_POINTER, TG_SLICE, TG_STRUCT,
  TG_ENUM, TG_FUNCTION, TG_INTERFACE,
} TypeGo;

// AST Node Type
typedef enum ast_node_type {
  ast_undefined = 0,
  ast_token, ast_node_literal,
  ast_node_expr_member, ast_node_expr_unary, ast_node_expr_bin, ast_node_expr,
  ast_node_stmt_assign, ast_node_stmt_return, ast_node_stmt_if, ast_node_stmt_while, ast_node_stmt_for, ast_node_stmt, ast_node_stmt_block,
  ast_node_var, ast_node_func_declare, ast_node_func, ast_node_method,
  ast_node_slice, ast_node_union, ast_node_enum, ast_node_struct, ast_node_interface,
  ast_node_file, ast_node_package, ast_node_program
} AST_NodeType;

struct ast_node_literal {
  TypeGo type_go;
  union {
    int8_t  val_int8;  int16_t  val_int16;  int32_t  val_int32;  int64_t  val_int64;
    uint8_t val_uint8; uint16_t val_uint16; uint32_t val_uint32; uint64_t val_uint64;
    float   val_float; double   val_double;
    char    val_ch;    char    *val_string;
    bool    val_bool;
  } literal;
};

struct ast_node_expr_member {
  AST_NodeType type_ds;
  uint64_t offset;
  union {
    AST_NodeSlice  slice;
    AST_NodeStruct struct;
  } data_structure;
};

struct ast_node_expr_unary {
  AST_NodeType type_operand;
  enum {
    UNOP_PLUS,    UNOP_MINUS,
    UNOP_LOG_NOT, UNOP_BIT_NOT,
    UNOP_DEREF,   UNOP_REF,
    UNOP_INCR,    UNOP_DECR
  } unary_operator;
  union {
    AST_NodeExpr       expr;
    AST_NodeStmtAssign stmt_assign;
  } operand;
};

struct ast_node_expr_bin {
  AST_NodeType type_expr_left,
               type_expr_right;
  enum {
    // ARITHMETIC
    BINOP_ARIT_PLUS, BINOP_ARIT_MINUS, BINOP_ARIT_MUL, BINOP_ARIT_DIV, BINOP_ARIT_MOD,
    BINOP_ARIT_PLUS_EQ, BINOP_ARIT_MINUS_EQ, BINOP_ARIT_MUL_EQ, BINOP_ARIT_DIV_EQ, BINOP_ARIT_MOD_EQ,
    // COMPARISON
    BINOP_COMP_EQ, BINOP_COMP_NEQ,
    BINOP_COMP_LTHAN, BINOP_COMP_GTHAN, BINOP_COMP_LTHAN_EQ, BINOP_COMP_GTHAN_EQ,
    // LOGICAL
    BINOP_LOG_AND, BINOP_LOG_OR,
    // BITWISE ARITHMETIC
    BINOP_BIT_AND, BINOP_BIT_OR, BINOP_BIT_XOR, BINOP_BIT_LSHIFT, BINOP_BIT_RSHIFT
    BINOP_BIT_AND_EQ, BINOP_BIT_OR_EQ, BINOP_BIT_XOR_EQ, BINOP_BIT_LSHIFT_EQ, BINOP_BIT_RSHIFT_EQ
  } expr_operator;
  union {
    AST_NodeExpr       expr;
    AST_NodeStmtAssign stmt_assign;
  } expr_left;
  union {
    AST_NodeExpr       expr;
    AST_NodeStmtAssign stmt_assign;
  } expr_right;
};

struct ast_node_expr {
  AST_NodeType type_expr;
  union {
    AST_NodeLiteral    literal;
    AST_NodeVar        var;
    AST_NodeSlice      slice;
    AST_NodeExprMember expr_member;
    AST_NodeExprUnary  expr_unary;
    AST_NodeExprBin    expr_bin;
  }
};

struct ast_node_stmt_assign {
  AST_NodeVar  var;
  AST_NodeExpr expr;
};

struct ast_node_stmt_return {
  AST_NodeType type_stmt;
  union {
    AST_NodeExpr       expr;
    AST_NodeStmtAssign stmt_assign;
  } return;
};

struct ast_node_stmt_if {
  bool has_else;
  uint32_t s_elseif;
  AST_NodeVar       init;
  AST_NodeExpr      condition;
  AST_NodeStmtBlock if_block;
  AST_NodeStmtIf    elseif;
  AST_NodeStmtBlock else_block;
};

struct ast_node_stmt_loop_while {
  bool loop_while;
  AST_NodeExpr      condition;
  AST_NodeStmtBlock block;
};

struct ast_node_stmt_loop_for {
  uint32_t s_init,
           s_update;
  AST_NodeVar        init;
  AST_NodeExpr       condition;
  AST_NodeStmtAssign update;
  AST_NodeStmtBlock  block;
};

struct ast_node_stmt {
  AST_NodeStmtType type_stmt;
  union {
    struct ast_node_expr          expr;
    struct ast_node_stmt_assign   stmt_assign;
    struct ast_node_stmt_return   stmt_return;
    struct ast_node_stmt_if       stmt_if;
    struct ast_stmt_loop_while    stmt_loop_while;
    struct ast_node_stmt_loop_for stmt_loop_for;
    struct ast_node_stmt_block    stmt_block;
  } stmt;
};

struct ast_node_stmt_block {
  uint32_t s_stmts;
  AST_NodeStmt stmts;
};

union ast_node_type_ident {
  TypeGo type_go;
  Token  type_custom;
};

struct ast_node_var {
  Token  token;
  bool   is_type_go; 
  AST_NodeTypeIdent type_var;
  AST_NodeExpr      value;
};

struct ast_node_func_declare {
  Token    func;
  uint32_t s_args,
           s_return; 
  bool    *is_type_go;
  AST_NodeVar       args;
  AST_NodeTypeIdent type_return;
};

struct ast_node_func {
  AST_NodeFuncDeclare declare;
  AST_NodeStmtBlock   block;
  AST_NodeStmtReturn  return;
};

struct ast_node_slice {
  uint64_t s_slice;
  AST_NodeVar var;
};

struct ast_node_interface {
  TypeGo   type_go;
  Token    interface;
  uint32_t s_methods;
  AST_NodeFuncDeclare methods;
};

struct ast_node_union {
  Token union;
  uint32_t s_types;
  AST_NodeTypeIdent types;
  TokenArray        values;
};

struct ast_node_enum {
  Token    enum;
  bool     iota;
  uint32_t s_values,
           s_assign;
  TokenArray values;
  TokenArray assign;
};

struct ast_node_method {
  AST_NodeStruct struct;
  AST_NodeFunc   func;
};

struct ast_node_struct {
  Token struct;
  uint32_t s_types;
  AST_NodeTypeIdent types;
};

struct ast_node_import {
  uint32_t s_imports;
  const char *imports;
};

struct ast_node_file {
  uint32_t s_file_blocks;
  AST_NodeType *type_file_blocks;
  union {
    AST_NodeImport      import; 
    AST_NodeEnum        enum;
    AST_NodeUnion       union;
    AST_NodeStruct      struct;
    AST_NodeMethod      method;
    AST_NodeVar         var;
    AST_NodeFunc        func;
    AST_NodeFuncDeclare func_declare;
  } *file_blocks;
};

struct ast_node_package {
  Token    package;
  uint32_t s_files; 
  AST_NodeFile files;
};

struct ast_node_program {
  uint32_t s_packages;
  AST_NodePackage packages;
  AST_NodeFunc    main;
};

static void goc_parser_parse_file(AST_NodeProgram program, TokenArray array, uint64_t *word_ptr, bool main_file);

static void _goc_parser_find_package_dependencies();

// AST Node Objects
static AST_NodeImport      _goc_parser_ast_node_import_create(uint32_t s_imports, const char *imports);
static AST_NodeEnum        _goc_parser_ast_node_enum_create(
  Token enum, bool iota, uint32_t s_values, uint32_t s_assign, TokenArray values, TokenArray assign
);
static AST_NodeUnion       _goc_parser_ast_node_union_create(Token union, uint32_t s_types, AST_NodeTypeIdent types, TokenArray values);
static AST_NodeStruct      _goc_parser_ast_node_struct_create(Token struct, AST_NodeTypeIdent types, uint32_t s_types); 
static AST_NodeMethod      _goc_parser_ast_node_method_create(AST_NodeStruct struct, AST_NodeFunc func);
static AST_NodeVar         _goc_parser_ast_node_var_create(Token token, bool is_type_go, AST_NodeTypeIdent type_var, AST_NodeExpr value);
static AST_NodeFunc        _goc_parser_ast_node_func_create(
  AST_NodeFuncDeclare declare, AST_NodeStmtBlock block, AST_NodeStmtReturn return
);
static AST_NodeFuncDeclare _goc_parser_ast_node_func_declare_create(
  Token func, uint32_t s_args, uint32_t s_return, bool *is_type_go, AST_NodeVar args, AST_NodeTypeIdent type_return
);

// AST Node Statements
static AST_NodeStmtBlock   _goc_parser_ast_node_stmt_block_create(AST_NodeType *type_stmts, void *stmts, uint64_t s_stmts);
static AST_NodeStmt        _goc_parser_ast_node_stmt_create(AST_NodeType type_stmt, void *stmt);
static AST_NodeStmtFor     _goc_parser_ast_node_stmt_loop_for_create(
  uint32_t s_init, uint64_t s_update, AST_NodeVar init, AST_NodeExpr condition, AST_NodeStmtAssign update, AST_NodeStmtBlock block
);
static AST_NodeStmtWhile   _goc_parser_ast_node_stmt_loop_while_create(
  bool loop_while, struct ast_node_expr *condition, struct ast_node_stmt_block *block
);
static AST_NodeStmtIf      _goc_parser_ast_node_stmt_if_create(
  bool has_else, uint32_t s_elseif, AST_NodeType type_init, union ast_node_init *init, struct ast_node_expr *condition,
  struct ast_node_stmt_block *if_block, struct ast_node_stmt_if *elseif, struct ast_node_stmt_block *else_block
);
static AST_NodeStmtReturn  _goc_parser_ast_node_stmt_return_create(AST_NodeType type_stmt, void *return);
static AST_NodeStmtAssign  _goc_parser_ast_node_stmt_assign_create(struct ast_node_var *var, struct ast_node_stmt_expr *assign);

// AST Node Expression
static AST_NodeExpr        _goc_parser_ast_node_expr_create(AST_NodeType type_node, void *expr);
static AST_NodeExprBin     _goc_parser_ast_node_expr_bin_create(
  AST_NodeType type_expr_left, AST_NodeType type_expr_right, Token bin_op, void *expr_left, void *expr_right
);
static AST_NodeExprUnary   _goc_parser_ast_node_expr_unary_create(AST_NodeType type_expr, Token unary_op, void *operand);
static AST_NodeExprMember  _goc_parser_ast_node_expr_member_create(AST_NodeType type_ds, uint64_t offset, void *data_structure);

// AST Node Literal
static AST_NodeLiteral     _goc_parser_ast_node_literal(TypeGo type_go, void *literal);

// AST Node Memory Allocation
static void  *_goc_parser_ast_node_create(AST_NodeType type, uint64_t s_nodes);
static uint64_t _goc_parser_ast_node_size(AST_NodeType type);

static void goc_parser_print_error(
  void goc_error_func(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word),
  Token token
);

#endif // !GOC_PARSER_PRIVATE_H
