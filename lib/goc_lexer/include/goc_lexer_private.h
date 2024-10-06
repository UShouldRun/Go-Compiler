#ifndef GOC_LEXER_PRIVATE_H
#define GOC_LEXER_PRIVATE_H

// ==========================================================# PRIVATE #==================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "goc_error.h"

// ====# FILE MODE #====

#define READ  "r"
#define WRITE "w"

// ====# OPERATORS #====

#define CHAR_NULL       '\0'
#define CHAR_NEW_LINE   '\n'
#define CHAR_TAB        '\t'
#define CHAR_EOF        -1
#define CHAR_WSPACE     ' '

#define CHAR_LBRACE     '{'
#define CHAR_RBRACE     '}'
#define CHAR_LPAREN     '('
#define CHAR_RPAREN     ')'
#define CHAR_LSQPAREN   '['
#define CHAR_RSQPAREN   ']'

#define CHAR_SEMICOLON  ';'
#define CHAR_COMMA      ','
#define CHAR_COLON      ':'
#define CHAR_QUOTE      '"'
#define CHAR_APOST      39
#define CHAR_AMPER      '&'
#define CHAR_BAR        '|'
#define CHAR_QUESTMARK  '?'
#define CHAR_PERCENT    '%'
#define CHAR_PERIOD     '.'
#define CHAR_BANG       '!'
#define CHAR_UNDER      '_'

#define CHAR_EQUAL      '='
#define CHAR_PLUS       '+'
#define CHAR_MINUS      '-'
#define CHAR_STAR       '*'
#define CHAR_SLASH      '/'
#define CHAR_GTHAN      '>'
#define CHAR_LTHAN      '<'

#define CHAR_ZERO       '0'
#define CHAR_HEXA       'x'

// ====# KEYWORDS #====

#define KEYWORD_PACKAGE "package"
#define KEYWORD_IMPORT  "import"
#define KEYWORD_TYPE    "typedef"

#define KEYWORD_FUNC    "func"
#define KEYWORD_STRUCT  "struct"
#define KEYWORD_INTER   "interface"
#define KEYWORD_ENUM    "enum"
#define KEYWORD_UNION   "union"

#define KEYWORD_NIL     "nil"
#define KEYWORD_IOTA    "iota"
#define KEYWORD_RETURN  "return"
#define KEYWORD_VAR     "var"
#define KEYWORD_CONST   "const"

#define KEYWORD_INT     "int"
#define KEYWORD_INT8    "int8"
#define KEYWORD_INT16   "int16"
#define KEYWORD_INT32   "int32"
#define KEYWORD_INT64   "int64"

#define KEYWORD_UINT    "uint"
#define KEYWORD_UINT8   "uint8"
#define KEYWORD_UINT16  "uint16"
#define KEYWORD_UINT32  "uint32"
#define KEYWORD_UINT64  "uint64"

#define KEYWORD_FLOAT   "float"
#define KEYWORD_DOUBLE  "double"

#define KEYWORD_CHAR    "char"
#define KEYWORD_STRING  "string"

#define KEYWORD_BOOL    "bool"
#define KEYWORD_TRUE    "true"
#define KEYWORD_FALSE   "false"

#define KEYWORD_IF      "if"
#define KEYWORD_ELSE    "else"

#define KEYWORD_DO      "do"
#define KEYWORD_WHILE   "while"
#define KEYWORD_FOR     "for"
#define KEYWORD_RANGE   "range"

// =======# BASE #========

#define BASE_02         2
#define BASE_10         10
#define BASE_16         16

// =====# CONSTANTS #=====

#define TOKEN_TEXT_MAX_SIZE 255
#define TOKEN_MAX_PEEK      8
#define TOKEN_VALUE_NIL     "nil"

static const size_t token_size_init = 1024;
static const size_t token_text_max_size = 255;
static const size_t token_str_max_size = 255;

struct token_pos {
  size_t line, rel, abs, s_word;
};

union token_value {
  int64_t num_lit;
  double  real_lit;
  char    text[TOKEN_TEXT_MAX_SIZE];
};

struct token {
  enum token_type   type;
  struct token_pos  pos;
  union token_value value;
};

struct token_array {
  Token tokens;
  size_t s_tokens;
};

static TokenArray  goc_lexer_token_array_create(size_t s_tokens);

static TokenType   goc_lexer_get_token(FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value);
static const char *goc_lexer_token_type_match_str(TokenType type);

static void        goc_lexer_update_token_pos(struct token_pos *global, struct token_pos *pos, const char ch);
static char        goc_lexer_peek(FILE *file);
static char        goc_lexer_peek_n(FILE *file, uint32_t n);
static char        goc_lexer_consume( FILE *file, struct token_pos *global, struct token_pos *pos);
static void        goc_lexer_unconsume_char(FILE *file, struct token_pos *global, struct token_pos *pos, const char ch);
static char        goc_lexer_consume_wspace(FILE *file, struct token_pos *global, struct token_pos *pos);
static bool        goc_lexer_consume_char(FILE *file, struct token_pos *global, struct token_pos *pos, const char ch);
static char        goc_lexer_consume_comment(FILE *file, struct token_pos *global, struct token_pos *pos, char ch);
static TokenType   goc_lexer_consume_number(FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value);
static TokenType   goc_lexer_consume_string_lit(
  FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value, char ch
);
static TokenType   goc_lexer_consume_char_lit(
  FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value, char ch
);

static uint32_t    goc_lexer_base_convert(FILE *file, struct token_pos *pos, char ch, uint32_t base);
static bool        goc_lexer_base_alpha(char ch);

static bool        goc_lexer_comment_block_end(char ch, char next);

static bool        goc_lexer_ident(char ch);
static bool        goc_lexer_ident_middle(char ch);
static bool        goc_lexer_skip(char ch);

static bool        goc_lexer_null_iterator(FILE *file, struct token_pos *global, struct token_pos *pos, char ch);
static bool        goc_lexer_auto_assign(FILE *file, struct token_pos *global, struct token_pos *pos, char ch);
static bool        goc_lexer_double_equal(FILE *file, struct token_pos *global, struct token_pos *pos, char ch);
static bool        goc_lexer_increment(FILE *file, struct token_pos *global, struct token_pos *pos, char ch);

static bool        goc_lexer_package(char *word);
static bool        goc_lexer_import(char *word);
static bool        goc_lexer_typedef(char *word);

static bool        goc_lexer_func(char *word);
static bool        goc_lexer_struct(char *word);
static bool        goc_lexer_interface(char *word);
static bool        goc_lexer_enum(char *word);
static bool        goc_lexer_union(char *word);

static bool        goc_lexer_nil(char *word);
static bool        goc_lexer_iota(char *word);
static bool        goc_lexer_return(char *word);
static bool        goc_lexer_var(char *word);
static bool        goc_lexer_const(char *word);

static bool        goc_lexer_int8(char *word);
static bool        goc_lexer_int16(char *word);
static bool        goc_lexer_int32(char *word);
static bool        goc_lexer_int64(char *word);

static bool        goc_lexer_uint8(char *word);
static bool        goc_lexer_uint16(char *word);
static bool        goc_lexer_uint32(char *word);
static bool        goc_lexer_uint64(char *word);

static bool        goc_lexer_float(char *word);
static bool        goc_lexer_double(char *word);

static bool        goc_lexer_char(char *word);
static bool        goc_lexer_string(char *word);

static bool        goc_lexer_bool(char *word);
static bool        goc_lexer_true(char *word);
static bool        goc_lexer_false(char *word);

static bool        goc_lexer_if(char *word);
static bool        goc_lexer_else(char *word);

static bool        goc_lexer_do(char *word);
static bool        goc_lexer_while(char *word);
static bool        goc_lexer_for(char *word);
static bool        goc_lexer_range(char *word);

static double      pow_int(double value, int exp);

#endif // !GOC_LEXER_PRIVATE_H
