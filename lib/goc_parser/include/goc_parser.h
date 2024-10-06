#ifndef GOC_PARSER_H
#define GOC_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "goc_error.h"
#include "goc_lexer.h"

typedef struct ast_node_program         *AST_NodeProgram;
typedef struct ast_node_package         *AST_NodePackage;
typedef struct ast_node_file            *AST_NodeFile;

typedef struct ast_node_struct          *AST_NodeStruct;
typedef struct ast_node_method          *AST_NodeMethod;
typedef struct ast_node_enum            *AST_NodeEnum;
typedef struct ast_node_union           *AST_NodeUnion;
typedef struct ast_node_interface       *AST_NodeInterface;
typedef struct ast_node_func            *AST_NodeFunc;
typedef struct ast_node_func            *AST_NodeVar;

typedef struct ast_node_func_declare    *AST_NodeVarDeclare;
typedef struct ast_node_var_declare     *AST_NodeVarDeclare;

typedef struct ast_node_stm_block       *AST_NodeStmBlock;
typedef struct ast_node_stmt            *AST_NodeStmt;
typedef struct ast_node_stm_loop_for    *AST_NodeStmLoopFor;
typedef union  ast_node_stmt_init       *AST_NodeStmtInit;
typedef struct ast_node_stm_loop_while  *AST_NodeStmLoopWhile;
typedef struct ast_node_stm_loop_do     *AST_NodeStmLoopDo;
typedef struct ast_node_stm_if          *AST_NodeStmIf;
typedef struct ast_node_stm_return      *AST_NodeStmReturn;
typedef struct ast_node_stm_assign      *AST_NodeStmAssign;
typedef struct ast_node_stm_expr        *AST_NodeStmExpr;

typedef struct ast_node_expr_bin        *AST_NodeExprBin;
typedef struct ast_node_expr_unary      *AST_NodeExprUnary;
typedef struct ast_node_expr_call       *AST_NodeExprCall;
typedef struct ast_node_expr_array      *AST_NodeExprArray;
typedef struct ast_node_expr_member     *AST_NodeExprMember;

typedef struct ast_node_result          *AST_NodeResult;
typedef struct ast_node_literal         *AST_NodeLiteral;

#define KEYWORD_MAIN "main"

AST_NodeProgram goc_parser(FILE *file, TokenArray array);

#endif // !GOC_PARSER_H
