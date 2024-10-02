NAME     = $(notdir $(PWD))

CC 			 = gcc
CFLAGS   = -Wall -Werror -Wpedantic

INCLUDE  = -I./include/ -I./../goc_error/include/
LIB      = -lgoc_error -L./../goc_error/build/
SRC 		 = ./src/goc_lexer.c
OBJ 		 = $(BUILDDIR)goc_lexer.o

DEBUG   ?=
BUILDDIR = ./build/

.PHONY   = build clean builddir

build: ./src/goc_lexer.c builddir
	@$(CC) $(CFLAGS) $(DEBUG) -o $(OBJ) -c $(SRC) $(INCLUDE) $(LIB)
	@echo "Compiled $(SRC) into $(BUILDDIR)"
	@ar rcs $(BUILDDIR)libgoc_lexer.a $(OBJ)
	@echo "Successfully produced '$(NAME)' library"

clean:
	@if [ "$(wildcard $(BUILDDIR)*)" ]; then \
		rm -rf $(BUILDDIR)*; \
		echo "Successfully cleaned '$(NAME)'s $(BUILDDIR)"; \
	fi

builddir:
	@if [ ! -d $(BUILDDIR) ]; then \
		mkdir -p $(BUILDDIR); \
		echo "Successfully created $(BUILDDIR)"; \
	fi
