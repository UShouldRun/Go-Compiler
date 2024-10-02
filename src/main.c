#include <string.h>

#include "goc_error.h"
#include "goc_lexer.h"
// #include "goc_parser.h"

#define GOC_GO_FILE ".go"

bool goc_go_file(const char *file_name) {
  goc_error_assert(goc_error_nullptr, file_name != NULL);
  size_t len = strlen(file_name);
  if (len < 4)
    return false;
  return strcmp(file_name + len - strlen(GOC_GO_FILE), GOC_GO_FILE) == 0;
}

int main(int argc, char *argv[]) {
  goc_error_assert(goc_error_inval_arg, argc <= 2);
  goc_error_assert(goc_error_nullptr, argv[1] != NULL);

  const char *file_name = argv[1];
  goc_error_assert(goc_error_nullptr, goc_go_file(file_name) == true);

  TokenArray tokens = goc_lexer(file_name);
  goc_error_assert(goc_error_nullptr, tokens != NULL);

  fprintf(stdout, "File %s:\n", file_name);

  size_t s_tokens = goc_lexer_token_array_get_size(tokens);
  for (size_t i = 0; i < s_tokens; i++)
    fprintf(stdout, "%s\n", goc_lexer_token_to_str(goc_lexer_token_array_at(tokens, i)));

  return 0;
}
