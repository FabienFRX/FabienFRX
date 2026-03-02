##
## EPITECH PROJECT, 2025
## Wolfram
## File description:
## Makefile
##

## --------- COLOR ------##

DEFAULT	=	"\033[00m"
GREEN	=	"\033[1;32m"
TEAL	=	"\033[1;36m"
YELLOW	=	"\033[1;7;25;33m"
MAGENTA	=	"\033[1;3;4;35m"
ERROR	=	"\033[5;7;1;31m"

ECHO	=	echo -e

## -------- COMPIL ----##

STACK	=	stack

## ------- BIN ----------##

BINNAME	=	imageCompressor

## ------- BUILD --------##

BUILD_DIR =	$(shell stack path --local-install-root)/bin

## --------- RULES --------##

all:
	@$(STACK) build && \
	cp $(BUILD_DIR)/$(BINNAME)-exe $(BINNAME) && \
	$(ECHO) $(GREEN) "[OK]" \
		$(TEAL) "Build successful: " \
		$(BINNAME) $(DEFAULT) || \
	$(ECHO) $(ERROR) "[ERROR] Build failed!" $(DEFAULT)

test_rule:
	stack test

clean:
	@$(STACK) clean && \
	rm -f src/Main && \
	$(ECHO) $(MAGENTA) "[OK] Cleaned project" $(DEFAULT)

fclean: clean
	@rm -f $(BINNAME) && \
	$(ECHO) $(MAGENTA) "[OK] Removed binary: $(BINNAME)" $(DEFAULT)

re: fclean all
