#include "goc_lexer.h"
#include "goc_lexer_private.h"

// ====================================================# PUBLIC #======================================================================

TokenArray goc_lexer(const char *file_name) {
  goc_error_assert(goc_error_nullptr, file_name != NULL);

  FILE *file = fopen(file_name, READ);
  if (file == NULL)
    goc_error_lexer_print_input_file(file_name);

  struct token_pos global = { 1, 0, 0, 0 };

  TokenArray tokens = goc_lexer_token_array_create(token_size_init);
  size_t s_tokens = 0;
  for (TokenType type = TT_BEGIN; type != TT_EOF; s_tokens++) {
    if (s_tokens >= tokens->s_tokens) {
      Token temp = (Token)realloc(tokens->tokens, 2 * s_tokens * sizeof(struct token));
      goc_error_assert(goc_error_mem_error, temp != NULL);
      tokens->s_tokens = 2 * s_tokens;
      tokens->tokens = temp;
    }
    type = goc_lexer_get_token(file, &global, &(tokens->tokens[s_tokens].pos), &(tokens->tokens[s_tokens].value));
    tokens->tokens[s_tokens].type = type;
  }

  if (s_tokens == 0) {
    goc_lexer_token_array_free(tokens);
    return NULL;
  } else if (s_tokens != tokens->s_tokens) {
    Token temp = (Token)realloc(tokens->tokens, s_tokens * sizeof(struct token));
    goc_error_assert(goc_error_mem_error, temp != NULL);
    tokens->s_tokens = s_tokens;
    tokens->tokens = temp;
  }

  return tokens;
}

Token goc_lexer_token_array_at(TokenArray token_array, size_t index) {
  return token_array ? ((index >= 0 && index < token_array->s_tokens) ? &(token_array->tokens[index]) : NULL) : NULL;
}

size_t goc_lexer_token_array_get_size(TokenArray token_array) {
  return !token_array ? 0 : token_array->s_tokens;
}

void goc_lexer_token_array_free(TokenArray token_array) {
  if (token_array == NULL)
    return;
  if (token_array->tokens)
    free(token_array->tokens);
  free(token_array);
}

const TokenType goc_lexer_token_get_token_type(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->type;
}

const size_t goc_lexer_token_get_pos_abs(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->pos.abs;
}

const size_t goc_lexer_token_get_pos_line(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->pos.line;
}

const size_t goc_lexer_token_get_pos_rel(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->pos.rel;
}

const size_t goc_lexer_token_get_pos_s_word(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->pos.s_word;
}

int64_t goc_lexer_token_get_value_number_literal(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->value.num_lit;
}

double goc_lexer_token_get_value_real_literal(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  return token->value.real_lit;
}

char *goc_lexer_token_get_value_text(Token token) {
  goc_error_assert(goc_error_nullptr, token != NULL);
  size_t len = strlen(token->value.text);
  char *cp = (char *)calloc(len, sizeof(char));
  if (cp == NULL)
    return NULL;
  strncpy(cp, token->value.text, len);
  return cp;
}

const char *goc_lexer_token_to_str(Token token) {
  if (token == NULL)
    return NULL;

  char *token_str = (char *)calloc(token_str_max_size, sizeof(char));
  if (token_str == NULL)
    return NULL;

  const char *token_type_str = goc_lexer_token_type_match_str(token->type);
  if (token_type_str == NULL)
    return NULL;

  char token_value_str[TOKEN_TEXT_MAX_SIZE + 2] = {0};
  switch (token->type) {
    case TT_IDENT: case TT_STRING_LIT: case TT_CHAR_LIT: {
      snprintf(token_value_str, TOKEN_TEXT_MAX_SIZE + 2, "%c%s%c", CHAR_LTHAN, token->value.text, CHAR_GTHAN);
      break;
    }
    case TT_NUM_LIT: {
      snprintf(token_value_str, token_str_max_size - 1, "%ld", token->value.num_lit);
      break;
    }
    case TT_REAL_LIT: {
      snprintf(token_value_str, token_str_max_size - 1, "%lf", token->value.real_lit);
      break;
    }
    default: {
      strncpy(token_value_str, TOKEN_VALUE_NIL, TOKEN_TEXT_MAX_SIZE / 2 + 1);
      break;
    }
  }

  snprintf(
    token_str,
    token_str_max_size - 1,
    "%sToken(%s\n  %sType%s(%s%s%s),\n  %sValue%s(%s%s%s),\n  %sPos%s(%sabs:%zu, line:%zu, rel:%zu, s_word:%zu%s)\n%s)%s",
    ANSI_COLORS[ANSI_COLOR_YELLOW], ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_WHITE], ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_RED], token_type_str, ANSI_COLORS[ANSI_COLOR_RESET], 
    ANSI_COLORS[ANSI_COLOR_WHITE], ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_GREEN], token_value_str, ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_WHITE], ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_CYAN], token->pos.abs, token->pos.line, token->pos.rel, token->pos.s_word, ANSI_COLORS[ANSI_COLOR_RESET],
    ANSI_COLORS[ANSI_COLOR_YELLOW], ANSI_COLORS[ANSI_COLOR_RESET]
  );

  return (const char *)token_str;
}

// ====================================================# PRIVATE #======================================================================

static TokenArray goc_lexer_token_array_create(size_t s_tokens) {
  if (s_tokens == 0)
    return NULL;

  TokenArray token_array = (TokenArray)malloc(sizeof(struct token_array));
  if (token_array == NULL)
    return NULL;

  Token tokens = (Token)calloc(s_tokens, sizeof(struct token));
  if (tokens == NULL) {
    free(token_array);
    return NULL;
  }

  token_array->tokens = tokens;
  token_array->s_tokens = s_tokens;
  return token_array;
}

static TokenType goc_lexer_get_token(FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  goc_error_assert(goc_error_nullptr, value != NULL);

  char ch;
  while ((ch = goc_lexer_consume_comment(file, global, pos, goc_lexer_consume_wspace(file, global, pos))) == CHAR_WSPACE);

  switch (ch) {
    case CHAR_LBRACE:     return TT_LBRACE;
    case CHAR_RBRACE:     return TT_RBRACE;
    case CHAR_LPAREN:     return TT_LPAREN;
    case CHAR_RPAREN:     return TT_RPAREN;
    case CHAR_LSQPAREN:   return TT_LSQPAREN;
    case CHAR_RSQPAREN:   return TT_RSQPAREN;

    case CHAR_SEMICOLON:  return TT_SEMICOLON;
    case CHAR_COMMA:      return TT_COMMA;
    case CHAR_QUESTMARK:  return TT_QUESTMARK;
    case CHAR_PERCENT:    return TT_PERCENT;
    case CHAR_BANG:       return TT_BANG;

    case CHAR_SLASH:      return TT_DIV;
    case CHAR_STAR:       return TT_STAR;
    case CHAR_GTHAN:      return TT_GTHAN;
    case CHAR_LTHAN:      return TT_LTHAN;

    case CHAR_EOF:        return TT_EOF;

    case CHAR_COLON:      return goc_lexer_auto_assign(file, global, pos, CHAR_COLON) ? TT_AUTO_ASSIGN : TT_COLON;
    case CHAR_EQUAL:      return goc_lexer_double_equal(file, global, pos, CHAR_EQUAL) ? TT_DOUBLE_EQUAL : TT_EQUAL;
    case CHAR_AMPER:      return goc_lexer_consume_char(file, global, pos, CHAR_AMPER) ? TT_AND : TT_AND_BIT_OP;
    case CHAR_BAR:        return goc_lexer_consume_char(file, global, pos, CHAR_BAR) ? TT_OR : TT_OR_BIT_OP;
    case CHAR_PLUS:       return goc_lexer_increment(file, global, pos, ch) ? TT_INCR : TT_PLUS;
    case CHAR_MINUS:      return goc_lexer_consume_char(file, global, pos, CHAR_GTHAN) ? TT_ARROW : TT_MINUS;
    case CHAR_QUOTE:      return goc_lexer_consume_string_lit(file, global, pos, value, ch);
    case CHAR_APOST:      return goc_lexer_consume_char_lit(file, global, pos, value, ch);

    case CHAR_PERIOD: {
      if (!isdigit(goc_lexer_peek(file)))
        return TT_PERIOD;
      goc_lexer_unconsume_char(file, global, pos, ch);
      return goc_lexer_consume_number(file, global, pos, value);
    }
 
    default: {
      if (isdigit(ch)) {
        goc_lexer_unconsume_char(file, global, pos, ch);
        return goc_lexer_consume_number(file, global, pos, value);
      } else if (goc_lexer_ident(ch)) {
        if (goc_lexer_null_iterator(file, global, pos, ch))
          return TT_NULL_ITERATOR;

        size_t index = 0;
        do {
          if (index > token_text_max_size)
            goc_error_lexer_print_buffer_overrun(file, pos->abs, pos->line, pos->rel + index, pos->s_word);
          if (index > 0)
            ch = goc_lexer_consume(file, global, pos);
          value->text[index++] = ch;
        } while (goc_lexer_ident_middle(goc_lexer_peek(file)));

        if (goc_lexer_package(value->text))
          return TT_PACKAGE;
        if (goc_lexer_import(value->text))
          return TT_IMPORT;
        if (goc_lexer_typedef(value->text))
          return TT_TYPE;

        if (goc_lexer_func(value->text))
          return TT_FUNC;
        if (goc_lexer_struct(value->text))
          return TT_STRUCT;
        if (goc_lexer_interface(value->text))
          return TT_INTERFACE;
        if (goc_lexer_enum(value->text))
          return TT_ENUM;
        if (goc_lexer_union(value->text))
          return TT_UNION;

        if (goc_lexer_nil(value->text))
          return TT_NIL;
        if (goc_lexer_iota(value->text))
          return TT_IOTA;
        if (goc_lexer_return(value->text))
          return TT_RETURN;
        if (goc_lexer_var(value->text))
          return TT_VAR;
        if (goc_lexer_const(value->text))
          return TT_CONST;

        if (goc_lexer_int8(value->text))
          return TT_INT8;
        if (goc_lexer_int16(value->text))
          return TT_INT16;
        if (goc_lexer_int32(value->text))
          return TT_INT32;
        if (goc_lexer_int64(value->text))
          return TT_INT64;

        if (goc_lexer_uint8(value->text))
          return TT_UINT8;
        if (goc_lexer_uint16(value->text))
          return TT_UINT16;
        if (goc_lexer_uint32(value->text))
          return TT_UINT32;
        if (goc_lexer_uint64(value->text))
          return TT_UINT64;

        if (goc_lexer_float(value->text))
          return TT_FLOAT;
        if (goc_lexer_double(value->text))
          return TT_DOUBLE;

        if (goc_lexer_char(value->text))
          return TT_CHAR;
        if (goc_lexer_string(value->text))
          return TT_STRING;

        if (goc_lexer_bool(value->text))
          return TT_BOOL;
        if (goc_lexer_true(value->text))
          return TT_TRUE_LIT;
        if (goc_lexer_false(value->text))
          return TT_FALSE_LIT;

        if (goc_lexer_if(value->text))
          return TT_IF;
        if (goc_lexer_else(value->text))
          return TT_ELSE;
        
        if (goc_lexer_do(value->text))
          return TT_DO;
        if (goc_lexer_while(value->text))
          return TT_WHILE;
        if (goc_lexer_for(value->text))
          return TT_FOR;
        if (goc_lexer_range(value->text))
          return TT_RANGE;

        return TT_IDENT;
      }

      goc_error_lexer_print_invalid_ident(file, pos->abs, pos->line, pos->rel, pos->s_word);
    } 
  }

  return TT_UNKNOWN;
}

static const char *goc_lexer_token_type_match_str(TokenType type) {
  switch (type) {
    case TT_LBRACE:        return "TT_LBRACE";
    case TT_RBRACE:        return "TT_RBRACE";
    case TT_LPAREN:        return "TT_LPAREN";
    case TT_RPAREN:        return "TT_RPAREN";
    case TT_LSQPAREN:      return "TT_LSQPAREN";
    case TT_RSQPAREN:      return "TT_RSQPAREN";

    case TT_SEMICOLON:     return "TT_SEMICOLON";
    case TT_COMMA:         return "TT_COMMA";
    case TT_AND:           return "TT_AND";
    case TT_AND_BIT_OP:    return "TT_AND_BIT_OP";
    case TT_OR:            return "TT_OR";
    case TT_OR_BIT_OP:     return "TT_OR_BIT_OP";
    case TT_QUESTMARK:     return "TT_QUESTMARK";
    case TT_PERCENT:       return "TT_PERCENT";
    case TT_BANG:          return "TT_BANG";

    case TT_DIV:           return "TT_DIV";
    case TT_MINUS:         return "TT_MINUS";
    case TT_STAR:          return "TT_STAR";
    case TT_GTHAN:         return "TT_GTHAN";
    case TT_LTHAN:         return "TT_LTHAN";
    case TT_ARROW:         return "TT_ARROW";

    case TT_EOF:           return "TT_EOF";

    case TT_AUTO_ASSIGN:   return "TT_AUTO_ASSIGN";      
    case TT_COLON:         return "TT_COLON";
    case TT_DOUBLE_EQUAL:  return "TT_DOUBLE_EQUAL";
    case TT_EQUAL:         return "TT_EQUAL";
    case TT_PERIOD:        return "TT_PERIOD";
    case TT_INCR:          return "TT_INCR";
    case TT_PLUS:          return "TT_PLUS";

    case TT_NUM_LIT:       return "TT_NUM_LIT";
    case TT_REAL_LIT:      return "TT_REAL_LIT";
    case TT_STRING_LIT:    return "TT_STRING_LIT";
    case TT_CHAR_LIT:      return "TT_CHAR_LIT";

    case TT_INT8:          return "TT_INT8";
    case TT_INT16:         return "TT_INT16";
    case TT_INT32:         return "TT_INT32";
    case TT_INT64:         return "TT_INT64";

    case TT_UINT8:         return "TT_UINT8";
    case TT_UINT16:        return "TT_UINT16";
    case TT_UINT32:        return "TT_UINT32";
    case TT_UINT64:        return "TT_UINT64";

    case TT_FLOAT:         return "TT_FLOAT";
    case TT_DOUBLE:        return "TT_DOUBLE";

    case TT_CHAR:          return "TT_CHAR";
    case TT_STRING:        return "TT_STRING";
    
    case TT_BOOL:          return "TT_BOOL";
    case TT_TRUE_LIT:      return "TT_TRUE_LIT";
    case TT_FALSE_LIT:     return "TT_FALSE_LIT";

    case TT_IF:            return "TT_IF";
    case TT_ELSE:          return "TT_ELSE";

    case TT_SWITCH:        return "TT_SWITCH";

    case TT_WHILE:         return "TT_WHILE";
    case TT_FOR:           return "TT_FOR";
    case TT_DO:            return "TT_DO";
    case TT_NULL_ITERATOR: return "TT_NULL_ITERATOR";
    case TT_RANGE:         return "TT_RANGE";

    case TT_PACKAGE:       return "T_PACKAGE";
    case TT_IMPORT:        return "TT_IMPORT";
    case TT_TYPE:          return "TT_TYPE";

    case TT_FUNC:          return "TT_FUNC";
    case TT_STRUCT:        return "TT_STRUCT";
    case TT_INTERFACE:     return "TT_INTERFACE";
    case TT_ENUM:          return "TT_ENUM";
    case TT_UNION:         return "TT_UNION";

    case TT_NIL:           return "TT_NIL";
    case TT_IOTA:          return "TT_IOTA";
    case TT_RETURN:        return "TT_RETURN";
    case TT_VAR:           return "TT_VAR";
    case TT_CONST:         return "TT_CONST";
    case TT_IDENT:         return "TT_IDENT";
 
    default:               return "TT_UNKNOWN";
  }
}

static void goc_lexer_update_token_pos(struct token_pos *global, struct token_pos *pos, const char ch) {
  if (global == NULL)
    return;
  global->abs++;
  global->line += (ch == CHAR_NEW_LINE);
  global->rel = (ch == CHAR_NEW_LINE) ? 0 : global->rel + 1;

  if (pos == NULL)
    return;

  size_t s_word = pos->s_word;
  if (!goc_lexer_skip(ch))
    pos->s_word++;
  if (s_word > 0)
    return;
  pos->abs  = global->abs;
  pos->line = global->line;
  pos->rel  = global->rel;
}

static char goc_lexer_peek(FILE *file) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  char ch = fgetc(file);
  ungetc(ch, file);
  return ch;
}

static char goc_lexer_peek_n(FILE *file, uint32_t n) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_inval_arg, n <= TOKEN_MAX_PEEK);
  uint64_t current_pos = ftell(file);
  if (current_pos == -1)
    return CHAR_NULL;
  char peek_buffer[TOKEN_MAX_PEEK] = { CHAR_NULL };
  if (fgets(peek_buffer, n + 1, file) == NULL)
    return CHAR_NULL;
  fseek(file, current_pos, SEEK_SET);
  return peek_buffer[n - 1];
}

static char goc_lexer_consume(FILE *file, struct token_pos *global, struct token_pos *pos) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  char ch = fgetc(file);
  goc_lexer_update_token_pos(global, pos, ch);
  return ch;
}

static void goc_lexer_unconsume_char(FILE *file, struct token_pos *global, struct token_pos *pos, const char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);

  global->abs--;
  if (ch == CHAR_NEW_LINE) {
    global->line--;

    fseek(file, 0, SEEK_SET);
    char line_buffer[TOKEN_TEXT_MAX_SIZE];
    uint32_t current_line = 1;
    for (; fgets(line_buffer, sizeof(line_buffer), file); current_line++)
      if (current_line == global->line) {
        global->rel = strlen(line_buffer) - 1;
        break;
      }

    fseek(file, global->abs, SEEK_SET);
    ungetc(ch, file);
    return;
  }
  global->rel--;
  if (!goc_lexer_skip(ch))
    pos->s_word--;
  ungetc(ch, file);
}

static bool goc_lexer_consume_char(FILE *file, struct token_pos *global, struct token_pos *pos, const char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);

  char consumed = fgetc(file);
  if (consumed != ch) {
    ungetc(consumed, file);
    return false;
  }
  goc_lexer_update_token_pos(global, pos, ch);
  return true;
}

static char goc_lexer_consume_wspace(FILE *file, struct token_pos *global, struct token_pos *pos) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  while (goc_lexer_skip(goc_lexer_peek(file)))
    (void)goc_lexer_consume(file, global, pos);
  return goc_lexer_consume(file, global, pos);
}

static char goc_lexer_consume_comment(FILE *file, struct token_pos *global, struct token_pos *pos, char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);

  if (ch != CHAR_SLASH)
    return ch;

  switch (ch = goc_lexer_consume(file, global, pos)) {
    case CHAR_SLASH: {
      while ((ch = goc_lexer_consume(file, global, pos)) != CHAR_NEW_LINE);
      break;
    }
    case CHAR_STAR: {
      char next = goc_lexer_consume(file, global, pos);
      for (; !goc_lexer_comment_block_end(ch, next); next = goc_lexer_consume(file, global, pos))
        if (goc_lexer_comment_block_end(next, ch = goc_lexer_consume(file, global, pos)))
          break;
      ch = goc_lexer_consume(file, global, pos);
      break;
    }
    default:
      break;
  }

  return ch;
}

static TokenType goc_lexer_consume_number(FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value) { 
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  goc_error_assert(goc_error_nullptr, value != NULL);

  if (!goc_lexer_base_alpha(goc_lexer_peek(file)))
    goc_error_lexer_print_invalid_number(file, pos->abs, pos->line, pos->rel, pos->s_word);

  uint32_t base = BASE_10,
           digit = 0,
           exp = 0;
  int32_t  num_lit = 0;
  double   real_lit = 0;
  bool     consumed_period = false;

  if (goc_lexer_peek(file) == CHAR_ZERO && goc_lexer_peek_n(file, 2) == CHAR_HEXA) {
    base = BASE_16;
    (void)goc_lexer_consume_char(file, global, pos, CHAR_ZERO);
    (void)goc_lexer_consume_char(file, global, pos, CHAR_HEXA);
  }
 
  while (goc_lexer_base_alpha(goc_lexer_peek(file))) {
    char ch = goc_lexer_consume(file, global, pos);
    if (ch == CHAR_PERIOD && base == BASE_10) {
      consumed_period = true;
      real_lit = (double)num_lit;
      continue;
    }

    digit = goc_lexer_base_convert(file, pos, ch, base);
    if (consumed_period) {
      exp++;
      real_lit = real_lit + (double)digit / pow_int(base, exp);
      continue;
    }
    num_lit = num_lit * base + digit;
  }

  if (consumed_period) {
    value->real_lit = real_lit;
    return TT_REAL_LIT;
  }

  value->num_lit = num_lit;
  return TT_NUM_LIT;
}

static TokenType goc_lexer_consume_string_lit(
  FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value, char ch
) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  if (ch != CHAR_QUOTE)
    goc_error_lexer_print_invalid_string_lit(file, pos->line, pos->rel, pos->abs, pos->s_word);
  value->text[0] = ch;
  size_t index = 1;
  for (; (ch = goc_lexer_consume(file, global, pos)) != CHAR_QUOTE; value->text[index] = ch, index++)
    if (index + 1 >= TOKEN_TEXT_MAX_SIZE)
      goc_error_lexer_print_buffer_overrun(file, pos->line, pos->rel, pos->abs, pos->s_word);
  value->text[index] = ch;
  return TT_STRING_LIT;
}

static TokenType goc_lexer_consume_char_lit(
  FILE *file, struct token_pos *global, struct token_pos *pos, union token_value *value, char ch
) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  if (ch != CHAR_APOST)
    goc_error_lexer_print_invalid_char_lit(file, pos->line, pos->rel, pos->abs, pos->s_word);
  value->text[0] = ch;
  value->text[1] = goc_lexer_consume(file, global, pos);
  if ((ch = goc_lexer_consume(file, global, pos)) != CHAR_QUOTE)
    goc_error_lexer_print_invalid_char_lit(file, pos->line, pos->rel, pos->abs, pos->s_word);
  value->text[2] = ch;
  return TT_CHAR_LIT;
}

static uint32_t goc_lexer_base_convert(FILE *file, struct token_pos *pos, char ch, uint32_t base) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  goc_error_assert(goc_error_inval_arg, base == BASE_16 || base == BASE_10);
  if (!goc_lexer_base_alpha(ch))
    goc_error_lexer_print_invalid_number(file, pos->abs, pos->line, pos->rel, pos->s_word);
  return isdigit(ch) ? (ch - '0') : (islower(ch) ? (ch - 'a' + 10) : (ch - 'A' + 10));
}

static bool goc_lexer_base_alpha(char ch) {
  return (
    (isdigit(ch) || ch == CHAR_PERIOD) && BASE_10) || ((isdigit(ch) || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')) && BASE_16
  );
}

static bool goc_lexer_comment_block_end(char ch, char next) {
  return ch == CHAR_STAR && next == CHAR_SLASH;
}

static bool goc_lexer_ident(char ch) {
  return isalpha(ch) || ch == CHAR_UNDER;
}

static bool goc_lexer_ident_middle(char ch) {
  return isalpha(ch) || isdigit(ch) || ch == CHAR_UNDER;
}

static bool goc_lexer_skip(char ch) {
  return ch == CHAR_WSPACE || ch == CHAR_NEW_LINE || ch == CHAR_TAB;
}

static bool goc_lexer_null_iterator(FILE *file, struct token_pos *global, struct token_pos *pos, char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  return ch == CHAR_UNDER && goc_lexer_consume_char(file, global, pos, CHAR_WSPACE);
}

static bool goc_lexer_auto_assign(FILE *file, struct token_pos *global, struct token_pos *pos, char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  return ch == CHAR_COLON && goc_lexer_consume_char(file, global, pos, CHAR_EQUAL);
}

static bool goc_lexer_double_equal(FILE *file, struct token_pos *global, struct token_pos *pos, char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  return ch == CHAR_EQUAL && goc_lexer_consume_char(file, global, pos, CHAR_EQUAL);
}

static bool goc_lexer_increment(FILE *file, struct token_pos *global, struct token_pos *pos, char ch) {
  goc_error_assert(goc_error_nullptr, file != NULL);
  goc_error_assert(goc_error_nullptr, global != NULL);
  goc_error_assert(goc_error_nullptr, pos != NULL);
  return ch == CHAR_PLUS && goc_lexer_consume_char(file, global, pos, CHAR_PLUS);
}

static bool goc_lexer_package(char *word) {
  return word ? strcmp(word, KEYWORD_PACKAGE) == 0 : false;
}

static bool goc_lexer_import(char *word) {
  return word ? strcmp(word, KEYWORD_IMPORT) == 0 : false;
}

static bool goc_lexer_typedef(char *word) {
  return word ? strcmp(word, KEYWORD_TYPE) == 0 : false;
}

static bool goc_lexer_func(char *word) {
  return word ? strcmp(word, KEYWORD_FUNC) == 0 : false;
}

static bool goc_lexer_struct(char *word) {
  return word ? strcmp(word, KEYWORD_STRUCT) == 0 : false;
}

static bool goc_lexer_interface(char *word) {
  return word ? strcmp(word, KEYWORD_INTER) == 0 : false;
}

static bool goc_lexer_enum(char *word) {
  return word ? strcmp(word, KEYWORD_ENUM) == 0 : false;
}

static bool goc_lexer_union(char *word) {
  return word ? strcmp(word, KEYWORD_UNION) == 0 : false;
}

static bool goc_lexer_nil(char *word) {
  return word ? strcmp(word, KEYWORD_NIL) == 0 : false;
}

static bool goc_lexer_iota(char *word) {
  return word ? strcmp(word, KEYWORD_IOTA) == 0 : false;
}

static bool goc_lexer_return(char *word) {
  return word ? strcmp(word, KEYWORD_RETURN) == 0 : false;
}

static bool goc_lexer_var(char *word) {
  return word ? strcmp(word, KEYWORD_VAR) == 0 : false;
}

static bool goc_lexer_const(char *word) {
  return word ? strcmp(word, KEYWORD_CONST) == 0 : false;
}

static bool goc_lexer_int8(char *word) {
  return word ? strcmp(word, KEYWORD_INT8) == 0 : false;
}

static bool goc_lexer_int16(char *word) {
  return word ? strcmp(word, KEYWORD_INT16) == 0 : false;
}

static bool goc_lexer_int32(char *word) {
  return word ? (strcmp(word, KEYWORD_INT) == 0 || strcmp(word, KEYWORD_INT32) == 0) : false;
}

static bool goc_lexer_int64(char *word) {
  return word ? strcmp(word, KEYWORD_INT64) == 0 : false;
}

static bool goc_lexer_uint8(char *word) {
  return word ? strcmp(word, KEYWORD_UINT8) == 0 : false;
}

static bool goc_lexer_uint16(char *word) {
  return word ? strcmp(word, KEYWORD_UINT16) == 0 : false;
}

static bool goc_lexer_uint32(char *word) {
  return word ? (strcmp(word, KEYWORD_UINT) == 0 || strcmp(word, KEYWORD_UINT32) == 0) : false;
}

static bool goc_lexer_uint64(char *word) {
  return word ? strcmp(word, KEYWORD_UINT64) == 0 : false;
}

static bool goc_lexer_float(char *word) {
  return word ? strcmp(word, KEYWORD_FLOAT) == 0 : false;
}

static bool goc_lexer_double(char *word) {
  return word ? strcmp(word, KEYWORD_DOUBLE) == 0 : false;
}

static bool goc_lexer_char(char *word) {
  return word ? strcmp(word, KEYWORD_CHAR) == 0 : false;
}

static bool goc_lexer_string(char *word) {
  return word ? strcmp(word, KEYWORD_STRING) == 0 : false;
}

static bool goc_lexer_bool(char *word) {
  return word ? strcmp(word, KEYWORD_BOOL) == 0 : false;
}

static bool goc_lexer_true(char *word) {
  return word ? strcmp(word, KEYWORD_TRUE) == 0 : false;
}

static bool goc_lexer_false(char *word) {
  return word ? strcmp(word, KEYWORD_FALSE) == 0 : false;
}

static bool goc_lexer_if(char *word) {
  return word ? strcmp(word, KEYWORD_IF) == 0 : false;
}

static bool goc_lexer_else(char *word) {
  return word ? strcmp(word, KEYWORD_ELSE) == 0 : false;
}

static bool goc_lexer_do(char *word) {
  return word ? strcmp(word, KEYWORD_DO) == 0 : false;
}

static bool goc_lexer_while(char *word) {
  return word ? strcmp(word, KEYWORD_WHILE) == 0 : false;
}

static bool goc_lexer_for(char *word) {
  return word ? strcmp(word, KEYWORD_FOR) == 0 : false;
}

static bool goc_lexer_range(char *word) {
  return word ? strcmp(word, KEYWORD_RANGE) == 0 : false;
}

static double pow_int(double value, int32_t exp) {
  if (value == 0 || value == 1)
    return value;
  if (exp == 1)
    return value;
  double factor = exp < 0 ? 1 / value : value,
         res = 1;
  if (exp < 0)
    exp *= -1;
  for (; exp; res *= factor, exp--);
  return res;
}
