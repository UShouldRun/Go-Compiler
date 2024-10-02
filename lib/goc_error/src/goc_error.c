#include "goc_error.h"

static const char *_goc_error_match_error_type(GOC_Error error) {
  switch (error) {
    case goc_error_lexer_crash:           return ERROR_LEXER_CRASH;
    case goc_error_lexer_ioerror:         return ERROR_LEXER_IOERROR;
    case goc_error_lexer_invalid_syntax:  return ERROR_LEXER_INVALID_SYNTAX;

    default:                              return ERROR_UNKNOWN;
  }
}

static uint32_t intlen(int32_t value) {
  uint32_t len = 0;
  for (; value; value /= 10, len++);
  return len;
}

static void _goc_error_print(
  GOC_Error error, const char *msg, FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word
) {
  goc_error_assert(goc_error_nullptr, file != NULL);

  char line_buffer[MAX_LINE_SIZE];
  fseek(file, 0, SEEK_SET);
  uint32_t current_line = 1;
  for (; fgets(line_buffer, sizeof(line_buffer), file); current_line++)
    if (current_line == pos_line)
      break;

  size_t len = strlen(line_buffer);
  if (line_buffer[len - 1] == '\n')
    line_buffer[len - 1] = '\0';

  const char *error_msg = _goc_error_match_error_type(error);
  fprintf(
    stderr,
    "%s[%s] %s%s\n %u | %s\n %*s | %s%*c%s\n",
    ANSI_COLORS[ANSI_COLOR_RED],
    error_msg ? error_msg : "",
    msg,
    ANSI_COLORS[ANSI_COLOR_RESET],
    current_line,
    line_buffer,
    intlen(current_line), "",
    ANSI_COLORS[ANSI_COLOR_RED],
    /*0, CHAR_ERROR_UNDERLINE,*/
    pos_rel, CHAR_ERROR_POINTER,
    /*s_word - 1, CHAR_ERROR_UNDERLINE,*/
    ANSI_COLORS[ANSI_COLOR_RESET]
  );
  exit(error);
}

void goc_error_lexer_print_panic(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_invalid_syntax, "", file, pos_abs, pos_line, pos_rel, s_word);
}

void goc_error_lexer_print_buffer_overrun(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_crash, ERROR_LEXER_BUFFER_OVERRUN, file, pos_abs, pos_line, pos_rel, s_word);
}

void goc_error_lexer_print_input_file(const char *file_name) {
  fprintf(
    stderr,
    "| %s[%s] %s %s%s\n",
    ANSI_COLORS[ANSI_COLOR_RED],
    _goc_error_match_error_type(goc_error_lexer_ioerror),
    ERROR_LEXER_FILE_NO,
    file_name,
    ANSI_COLORS[ANSI_COLOR_RESET]
  );
  exit(goc_error_lexer_ioerror);
}

void goc_error_lexer_print_invalid_ident(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_invalid_syntax, ERROR_LEXER_INVALID_IDENT, file, pos_abs, pos_line, pos_rel, s_word);
}

void goc_error_lexer_print_invalid_number(FILE *file, uint32_t pos_abs, uint32_t pos_line, uint32_t pos_rel, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_invalid_syntax, ERROR_LEXER_INVALID_NUMBER, file, pos_abs, pos_line, pos_rel, s_word);
}

void goc_error_print_invalid_string_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_invalid_syntax, ERROR_LEXER_INVALID_STRING_LIT, file, pos_abs, pos_line, pos_rel, s_word);
}

void goc_error_print_invalid_char_lit(FILE *file, uint32_t pos_line, uint32_t pos_rel, uint32_t pos_abs, uint32_t s_word) {
  _goc_error_print(goc_error_lexer_invalid_syntax, ERROR_LEXER_INVALID_CHAR_LIT, file, pos_abs, pos_line, pos_rel, s_word);
}

void _goc_error_assert(GOC_Error error, const char *msg, const char *file, const char *func, uint32_t line) {
  fprintf(stderr, "[ASSERTION]: %s at %s (%s: %u)\nProgram exited with code %u\n", msg, func, file, line, error);
  exit(error);
}

const char *ANSI_COLORS[ANSI_COLOR_COUNT] = {
    "\033[0m",
    "\033[30m",
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m",
    "\033[90m",
    "\033[91m",
    "\033[92m",
    "\033[93m",
    "\033[94m",
    "\033[95m",
    "\033[96m",
    "\033[97m"
};
