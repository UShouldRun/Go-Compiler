#ifndef GOC_LEXER_H
#define GOC_LEXER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum token_type {
  TT_BEGIN = 0,
  TT_UNKNOWN,

  TT_STRING_LIT, TT_CHAR_LIT,
  TT_TRUE_LIT, TT_FALSE_LIT,

  TT_NUM_LIT, TT_REAL_LIT,
  TT_INT8, TT_INT16, TT_INT32, TT_INT64,
  TT_UINT8, TT_UINT16, TT_UINT32, TT_UINT64,
  TT_FLOAT, TT_DOUBLE,
  TT_CHAR, TT_STRING,
  TT_BOOL,

  TT_IF, TT_ELSE,
  TT_SWITCH, TT_CASE,
  TT_WHILE, TT_FOR, TT_DO,
  TT_NULL_ITERATOR, TT_RANGE,

  TT_PACKAGE,
  TT_IMPORT,
  TT_TYPE,

  TT_FUNC,
  TT_STRUCT,
  TT_INTERFACE,
  TT_ENUM,
  TT_UNION,

  TT_NIL,
  TT_IOTA,
  TT_RETURN,
  TT_VAR,
  TT_CONST,
  TT_IDENT,

  TT_LBRACE, TT_RBRACE,
  TT_LPAREN, TT_RPAREN,
  TT_LSQPAREN, TT_RSQPAREN,

  TT_SEMICOLON, TT_COMMA, TT_QUESTMARK,
  TT_COLON, TT_PERIOD, 

  // UNDECIDABLE UNTIL PARSING
  TT_PLUS, TT_MINUS, TT_STAR, TT_AMPER, TT_ARROW,
  // UNARY
  TT_UNOP_LOG_NOT, TT_UNOP_BIT_NOT,
  TT_UNOP_INCR,    TT_UNOP_DECR,
  // ARITHMETIC
  TT_BINOP_ARIT_DIV, TT_BINOP_ARIT_MOD,
  TT_BINOP_ARIT_PLUS_EQ, TT_BINOP_ARIT_MINUS_EQ, TT_BINOP_ARIT_MUL_EQ, TT_BINOP_ARIT_DIV_EQ, TT_BINOP_ARIT_MOD_EQ,
  // COMPARISON
  TT_BINOP_COMP_EQ, TT_BINOP_COMP_NEQ,
  TT_BINOP_COMP_LTHAN, TT_BINOP_COMP_GTHAN, TT_BINOP_COMP_LTHAN_EQ, TT_BINOP_COMP_GTHAN_EQ,
  // LOGICAL
  TT_BINOP_LOG_AND, TT_BINOP_LOG_OR,
  // BITWISE ARITHMETIC
  TT_BINOP_BIT_AND, TT_BINOP_BIT_OR, TT_BINOP_BIT_XOR, TT_BINOP_BIT_LSHIFT, TT_BINOP_BIT_RSHIFT,
  TT_BINOP_BIT_AND_EQ, TT_BINOP_BIT_OR_EQ, TT_BINOP_BIT_XOR_EQ, TT_BINOP_BIT_LSHIFT_EQ, TT_BINOP_BIT_RSHIFT_EQ,

  TT_ASSIGN, TT_AUTO_ASSIGN,

  TT_EOF
} TokenType;

typedef struct token       *Token;
typedef struct token_array *TokenArray;

TokenArray      goc_lexer(const char *file);
Token           goc_lexer_token_array_at(TokenArray token_array, size_t index);
size_t          goc_lexer_token_array_get_size(TokenArray token_array);
void            goc_lexer_token_array_free(TokenArray token_array);

const TokenType goc_lexer_token_get_token_type(Token token);
const size_t    goc_lexer_token_get_pos_abs(Token token); 
const size_t    goc_lexer_token_get_pos_line(Token token); 
const size_t    goc_lexer_token_get_pos_rel(Token token); 
const size_t    goc_lexer_token_get_pos_s_word(Token token); 
int64_t         goc_lexer_token_get_value_number_literal(Token token);
double          goc_lexer_token_get_value_real_literal(Token token);
char           *goc_lexer_token_get_value_text(Token token);

const char     *goc_lexer_token_to_str(Token token);

#endif // !GOC_LEXER_H
