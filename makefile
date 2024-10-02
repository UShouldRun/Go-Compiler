NAME 		 := $(notdir $(PWD))

CC   			= gcc
CFLAGS 		= -Wall -Werror -Wpedantic

INCLUDE 	= -I./lib/goc_lexer/include/ -I./lib/goc_error/include/
SRC				= ./src/main.c
LIB 			= -lgoc_lexer -L./lib/goc_lexer/build/ -lgoc_error -L./lib/goc_error/build/

DEBUG		 ?=
PREFIX   ?= .

BINDIR 	  = $(PREFIX)/bin/
BUILDDIR 	= ./build/

.PHONY  	= install all build uninstall clean_all clean bindir builddir build_goc_lexer build_goc_error clean_goc_lexer clean_goc_error
NO_PRINT  = --no-print-directory

all: build_goc_error build_goc_lexer build
clean_all: clean_goc_error clean_goc_lexer clean

install: build bindir
	@if [ -f $(BINDIR)$(NAME)]; then \
		echo "'$(NAME)' is already installed in $(BINDIR)"; \
	else \
		install -m 0755 $(BUILDDIR)$(NAME) $(BINDIR)$(NAME); \
		echo "Successfully installed '$(NAME)' in $(BINDIR)"; \
	fi

uninstall:
	@rm -f $(BINDIR)$(NAME)
	@echo "Uninstalled '$(NAME)' from $(BINDIR)"

build: $(SRC) builddir
	@$(CC) $(CFLAGS) $(DEBUG) -o $(BUILDDIR)$(NAME).o $(SRC) $(INCLUDE) $(LIB)
	@echo "Compiled '$(NAME)' into $(BUILDDIR)"

clean:
	@if [ "$(wildcard $(BUILDDIR)*)" ]; then \
		rm -rf $(BUILDDIR)*; \
		echo "Successfully cleaned '$(NAME)'s $(BUILDDIR)"; \
	fi
	
bindir:
	@if [ ! -d $(BINDIR) ]; then \
		mkdir -p $(BINDIR); \
		echo "Successfully created $(BINDIR)"; \
	fi

builddir:
	@if [ ! -d $(BUILDDIR) ]; then \
		mkdir -p $(BUILDDIR); \
		echo "Successfully created $(BUILDDIR)"; \
	fi

build_goc_lexer:
	@cd lib/goc_lexer/ && $(MAKE) $(NO_PRINT) clean && $(MAKE) $(NO_PRINT) DEBUG=$(DEBUG) build && cd ../../

build_goc_error:
	@cd lib/goc_error/ && $(MAKE) $(NO_PRINT) clean && $(MAKE) $(NO_PRINT) DEBUG=$(DEBUG) build && cd ../../

clean_goc_lexer:
	@cd lib/goc_lexer/ && $(MAKE) $(NO_PRINT) clean && cd ../../

clean_goc_error:
	@cd lib/goc_error/ && $(MAKE) $(NO_PRINT) clean && cd ../../
