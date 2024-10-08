#ifndef GOC_PARSER_H
#define GOC_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "goc_error.h"
#include "goc_lexer.h"

// =========# AST Nodes #============

// AST Node Super Objects
typedef struct ast_node_program         *AST_NodeProgram;
typedef struct ast_node_package         *AST_NodePackage;
typedef struct ast_node_file            *AST_NodeFile;

// AST Node Objects
typedef struct ast_node_struct       *AST_NodeStruct;
typedef struct ast_node_method       *AST_NodeMethod;
typedef struct ast_node_enum         *AST_NodeEnum;
typedef struct ast_node_union        *AST_NodeUnion;
typedef struct ast_node_slice        *AST_NodeSlice;
typedef struct ast_node_interface    *AST_NodeInterface;
typedef struct ast_node_func         *AST_NodeFunc;
typedef struct ast_node_func_declare *AST_NodeFuncDeclare;
typedef struct ast_node_var          *AST_NodeVar;
typedef struct ast_node_type_ident   *AST_NodeTypeIdent;

// AST Node Statements
typedef struct ast_node_stmt_block   *AST_NodeStmtBlock;
typedef struct ast_node_stmt         *AST_NodeStmt;
typedef struct ast_node_stmt_for     *AST_NodeStmtFor;
typedef union  ast_node_stmt_init    *AST_NodeStmtInit;
typedef struct ast_node_stmt_while   *AST_NodeStmtWhile;
typedef struct ast_node_stmt_if      *AST_NodeStmtIf;
typedef struct ast_node_stmt_return  *AST_NodeStmtReturn;
typedef struct ast_node_stmt_assign  *AST_NodeStmtAssign;

// AST Node Expression
typedef struct ast_node_expr         *AST_NodeExpr;
typedef struct ast_node_expr_bin     *AST_NodeExprBin;
typedef struct ast_node_expr_unary   *AST_NodeExprUnary;
typedef struct ast_node_expr_member  *AST_NodeExprMember;

// AST Node Single
typedef struct ast_node_literal      *AST_NodeLiteral;

#define KEYWORD_MAIN "main"

AST_NodeProgram goc_parser(FILE *file, TokenArray array);

#endif // !GOC_PARSER_H
