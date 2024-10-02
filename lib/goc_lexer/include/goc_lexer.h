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
  TT_SWITCH,
  TT_WHILE, TT_FOR, TT_DO, TT_NULL_ITERATOR, TT_RANGE,

  TT_PACKAGE,
  TT_IMPORT,
  TT_TYPE,
  TT_FUNC,
  TT_STRUCT,
  TT_INTERFACE,
  TT_ENUM,
  TT_NIL,
  TT_RETURN,
  TT_VAR,
  TT_CONST,
  TT_IDENT,

  TT_STAR,
  TT_ARROW,

  TT_LBRACE, TT_RBRACE,
  TT_LPAREN, TT_RPAREN,
  TT_LSQPAREN, TT_RSQPAREN,

  TT_SEMICOLON, TT_COMMA,
  TT_AND, TT_AND_BIT_OP, TT_OR, TT_OR_BIT_OP, TT_QUESTMARK,
  TT_COLON, TT_PERCENT, TT_PERIOD, TT_BANG,

  TT_EQUAL, TT_DOUBLE_EQUAL,
  TT_INCR, TT_AUTO_ASSIGN,

  TT_PLUS, TT_MINUS, TT_DIV,
  TT_GTHAN, TT_LTHAN,

  TT_EOF
} TokenType;

typedef struct token       *Token;
typedef struct token_array *TokenArray;

TokenArray  goc_lexer(const char *file);
Token       goc_lexer_token_array_at(TokenArray token_array, size_t index);
size_t      goc_lexer_token_array_get_size(TokenArray token_array);
void        goc_lexer_token_array_free(TokenArray token_array);
const char *goc_lexer_token_to_str(Token token);

#endif // !GOC_LEXER_H
