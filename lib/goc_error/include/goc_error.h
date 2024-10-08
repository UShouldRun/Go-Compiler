#ifndef GOC_ERROR_H
#define GOC_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum goc_error {
  goc_error_nil = 0,
  goc_error_nullptr,
  goc_error_inval_arg,
  goc_error_ioerror,
  goc_error_mem_error,

  goc_error_lexer_crash,
  goc_error_lexer_ioerror,
  goc_error_lexer_invalid_syntax,

  goc_error_parser_syntax_error,
  goc_error_parser_unexpected_end_of_input,
  goc_error_parser_mismatched_tokens,
  goc_error_parser_missing_token,
  goc_error_parser_ambiguous_grammar,
  goc_error_parser_invalid_token,
  goc_error_parser_unexpected_character,
  goc_error_parser_unclosed_string_or_comment,
  goc_error_parser_invalid_assignment,
  goc_error_parser_type_error,
  goc_error_parser_contextual_error,
  goc_error_parser_operator_precedence_error,
  goc_error_parser_overflow_or_invalid_numeric_literal,
  goc_error_parser_ast_undefined,
} GOC_Error;

#define GOC_CHAR_ERROR_POINTER                '^'
#define GOC_CHAR_ERROR_UNDERLINE              '~'

// GOC ERROR LEXER MESSAGES
#define GOC_ERROR_LEXER_CRASH                   "GOC Lexer Error: Crash"
#define GOC_ERROR_LEXER_IOERROR                 "GOC Lexer Error: IOError"
#define GOC_ERROR_LEXER_INVALID_SYNTAX          "GOC Lexer Error: Invalid Syntax"

#define GOC_ERROR_LEXER_FILE_NO                 "Could not open file"
#define GOC_ERROR_LEXER_BUFFER_OVERRUN          "Buffer overrun (identifier is too large)"
#define GOC_ERROR_LEXER_INVALID_IDENT           "Invalid Syntax Identifier"
#define GOC_ERROR_LEXER_INVALID_NUMBER          "Invalid Syntax Number"
#define GOC_ERROR_LEXER_INVALID_STRING_LIT      "Invalid Syntax String Literal"
#define GOC_ERROR_LEXER_INVALID_CHAR_LIT        "Invalid Syntax Char Literal"

// GOC ERROR PARSER MESSAGES
#define GOC_ERROR_PARSER_SYNTAX_ERROR                        "GOC Parser Error: Syntax Error"
#define GOC_ERROR_PARSER_UNEXPECTED_END_OF_INPUT             "GOC Parser Error: Unexpected End of Input"
#define GOC_ERROR_PARSER_MISMATCHED_TOKENS                   "GOC Parser Error: Mismatched Tokens"
#define GOC_ERROR_PARSER_MISSING_TOKEN                       "GOC Parser Error: Missing Token"
#define GOC_ERROR_PARSER_AMBIGUOUS_GRAMMAR                   "GOC Parser Error: Ambiguous Grammar"
#define GOC_ERROR_PARSER_INVALID_TOKEN                       "GOC Parser Error: Invalid Token"
#define GOC_ERROR_PARSER_UNEXPECTED_CHARACTER                "GOC Parser Error: Unexpected Character"
#define GOC_ERROR_PARSER_UNCLOSED_STRING_OR_COMMENT          "GOC Parser Error: Unclosed String or Comment"
#define GOC_ERROR_PARSER_INVALID_ASSIGNMENT                  "GOC Parser Error: Invalid Assignment"
#define GOC_ERROR_PARSER_TYPE_ERROR                          "GOC Parser Error: Type Error"
#define GOC_ERROR_PARSER_CONTEXTUAL_ERROR                    "GOC Parser Error: Contextual Error"
#define GOC_ERROR_PARSER_OPERATOR_PRECEDENCE_ERROR           "GOC Parser Error: Operator Precedence Error"
#define GOC_ERROR_PARSER_OVERFLOW_OR_INVALID_NUMERIC_LITERAL "GOC Parser Error: Overflow or Invalid Numeric Literal"
#define GOC_ERROR_PARSER_AST                                 "GOC Parser Error: AST Node"

#define GOC_ERROR_PARSER_PACKAGE_MAIN_NOT_FOUND              "Package main not found"
#define GOC_ERROR_PARSER_AST_UNDEFINED                       "Undefined AST node"

#define GOC_ERROR_UNKNOWN                                    "GOC Error: Unknown"

#define MAX_LINE_SIZE 512

// =========================================================# PRIVATE #================================================================

void _goc_error_assert(GOC_Error error, const char *msg, const char *file, const char *func, uint32_t line);

// ==========================================================# PUBLIC #================================================================

#define goc_error_assert(error, condidition) condidition ? (void)0 : _goc_error_assert(error, #condidition, __FILE__, __func__, __LINE__);
void goc_error_print_panic(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);

// GOC LEXER
void goc_error_lexer_print_input_file(const char *file_name);
void goc_error_lexer_print_buffer_overrun(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_lexer_print_invalid_ident(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_lexer_print_invalid_number(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_lexer_print_invalid_string_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);
void goc_error_lexer_print_invalid_char_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);

// GOC PARSER
void goc_error_parser_print_package_main_not_found(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);
void goc_error_parser_print_ast_undefined(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);

typedef enum {
    ANSI_COLOR_RESET,
    ANSI_COLOR_BLACK,
    ANSI_COLOR_RED,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_YELLOW,
    ANSI_COLOR_BLUE,
    ANSI_COLOR_MAGENTA,
    ANSI_COLOR_CYAN,
    ANSI_COLOR_WHITE,
    ANSI_COLOR_BRIGHT_BLACK,
    ANSI_COLOR_BRIGHT_RED,
    ANSI_COLOR_BRIGHT_GREEN,
    ANSI_COLOR_BRIGHT_YELLOW,
    ANSI_COLOR_BRIGHT_BLUE,
    ANSI_COLOR_BRIGHT_MAGENTA,
    ANSI_COLOR_BRIGHT_CYAN,
    ANSI_COLOR_BRIGHT_WHITE,
    ANSI_COLOR_COUNT
} ANSI_Color;

extern const char *ANSI_COLORS[ANSI_COLOR_COUNT];

#endif // !GOC_ERROR_H
