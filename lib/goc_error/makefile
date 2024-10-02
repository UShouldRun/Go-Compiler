NAME 			= $(notdir $(PWD))

CC 		 		= gcc
CFLAGS 		= -Wall -Werror -Wpedantic

BUILDDIR 	= ./build/
INCLUDE 	= -I./include/
SRC 			= ./src/goc_error.c
OBJ 			= $(BUILDDIR)goc_error.o
LIB 		  = $(BUILDDIR)libgoc_error.a

DEBUG		 ?=

.PHONY 		= build clean builddir

build: $(SRC) builddir
	@$(CC) $(CFLAGS) $(DEBUG) -o $(OBJ) -c $(SRC) $(INCLUDE)
	@echo "Compiled $(SRC) into $(BUILDDIR)"
	@ar rcs $(LIB) $(OBJ)
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
