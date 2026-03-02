##
## EPITECH PROJECT, 2025
## glados
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

BINNAME	=	glados
COMPILER_BIN = glados-compiler
VM_BIN = glados-vm

## ------- BUILD --------##

BUILD_DIR =	$(shell stack path --local-install-root --allow-different-user)/bin

## --------- RULES --------##

all:
	@$(STACK) build --allow-different-user && \
	cp $(BUILD_DIR)/$(COMPILER_BIN) $(COMPILER_BIN) && \
	cp $(BUILD_DIR)/$(VM_BIN) $(VM_BIN) && \
	$(ECHO) $(GREEN) "[OK]" \
		$(TEAL) "Build successful: " \
		$(COMPILER_BIN) $(VM_BIN) $(DEFAULT) || \
	$(ECHO) $(ERROR) "[ERROR] Build failed!" $(DEFAULT)

test_rule:
	stack test --allow-different-user

clean:
	@$(STACK) clean --allow-different-user && \
	rm -f src/Main && \
	rm -f $(BINNAME) $(COMPILER_BIN) $(VM_BIN) && \
	$(ECHO) $(MAGENTA) "[OK] Cleaned project" $(DEFAULT)

fclean: clean
	@rm -f $(BINNAME) $(COMPILER_BIN) $(VM_BIN) && \
	rm -f src/Main && \
	$(ECHO) $(MAGENTA) "[OK] Removed binaries" $(DEFAULT)

re: fclean all

.PHONY: all clean fclean re test_rule