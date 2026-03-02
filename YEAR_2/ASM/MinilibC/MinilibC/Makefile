##
## EPITECH PROJECT, 2025
## MinilibC
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

ASM	=	nasm

CC	=	ld

## ------- DIR --------##
SRCDIR	=	./src

## -------- SRC -------##

SRC	=	$(SRCDIR)/strlen.asm		\
		$(SRCDIR)/strchr.asm		\
		$(SRCDIR)/strrchr.asm		\
		$(SRCDIR)/memset.asm		\
		$(SRCDIR)/memcpy.asm		\
		$(SRCDIR)/strcmp.asm		\
		$(SRCDIR)/memmove.asm		\
		$(SRCDIR)/strncmp.asm		\
		$(SRCDIR)/strcasecmp.asm	\
		
OBJ	=	$(SRC:.asm=.o)

## ------- FLAGS --------##

cflags.common	:=	-shared -fno-builtin
cflags.debug	:=
cflags.release	:=
cflags.tests	:=

ldflags.common	:=
ldflags.debug	:=
ldflags.release	:=
ldflags.tests	:=

CFLAGS	:=	${cflags.${BUILD}} ${cflags.common}
LDFLAGS	:=	${ldflags.${BUILD}} ${ldflags.common}
ASMFLAGS	=	-f elf64

## ------- BIN ----------##

BINNAME	=	libasm.so

## ------- BUILD ----------##

BUILD   =   release

## --------- RULES --------##

all:
	@make -s $(BINNAME) && \
	$(ECHO) $(GREEN) "[OK]"$(TEAL)"  Done : " $@ $(DEFAULT)  || \
	$(ECHO) $(ERROR) "[ERROR]" $(YELLOW) $(BINNAME) $(DEFAULT)

$(BINNAME)	:	$(OBJ)
				@$(CC) $(CFLAGS) -o $(BINNAME) $(OBJ)

%.o	:	%.asm
		@$(ASM) -o $@ $< $(ASMFLAGS) && \
		$(ECHO) $(GREEN) "[OK] " $(DEFAULT) $(TEAL) $<  $(DEFAULT)  " -----> " $(GREEN) $@ $(DEFAULT) || \
		$(ECHO) $(ERROR) " [ERROR] can't find " $(YELLOW) $^ $(DEFAULT)

clean:
	@$(foreach i, $(OBJ), $(shell rm -rf $(i)) echo -e $(MAGENTA) "\tRemoved:  $(i)" $(DEFAULT);)

fclean:	clean
	@rm -rf $(BINNAME) && \
	$(ECHO) $(GREEN) "[OK]"$(TEAL)"  Done : " $@ $(DEFAULT)  || \
	$(ECHO) $(ERROR) "[ERROR]" $(YELLOW) $(BINNAME) $(DEFAULT)

re:	fclean all
