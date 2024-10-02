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
} GOC_Error;

#define CHAR_ERROR_POINTER                '^'
#define CHAR_ERROR_UNDERLINE              '~'

#define ERROR_LEXER_FILE_NO               "Could not open file"
#define ERROR_LEXER_BUFFER_OVERRUN        "Buffer overrun (identifier is too large)"
#define ERROR_LEXER_INVALID_IDENT         "Invalid Syntax Identifier"
#define ERROR_LEXER_INVALID_NUMBER        "Invalid Syntax Number"
#define ERROR_LEXER_INVALID_STRING_LIT    "Invalid Syntax String Literal"
#define ERROR_LEXER_INVALID_CHAR_LIT      "Invalid Syntax Char Literal"

#define ERROR_LEXER_CRASH                 "GOC Lexer Error: Crash"
#define ERROR_LEXER_IOERROR               "GOC Lexer Error: IOError"
#define ERROR_LEXER_INVALID_SYNTAX        "GOC Lexer Error: Invalid Syntax"

#define ERROR_UNKNOWN                     "GOC Error: Unknown"

#define MAX_LINE_SIZE                     512

// =========================================================# PRIVATE #================================================================

void _goc_error_assert(GOC_Error error, const char *msg, const char *file, const char *func, uint32_t line);

// ==========================================================# PUBLIC #================================================================

#define goc_error_assert(error, condidition) condidition ? (void)0 : _goc_error_assert(error, #condidition, __FILE__, __func__, __LINE__);

void goc_error_lexer_print_panic(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_lexer_print_buffer_overrun(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);

void goc_error_lexer_print_input_file(const char *file_name);

void goc_error_lexer_print_invalid_ident(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_lexer_print_invalid_number(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word);
void goc_error_print_invalid_string_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);
void goc_error_print_invalid_char_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word);

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
