##
## EPITECH PROJECT, 2024
## B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	src/main.c					\
		src/display_help.c			\
		src/option_n.c				\
		src/option_p.c				\
		src/palindrome_function.c	\
		src/function_base.c			\

OBJ	=	$(SRC:.c=.o)

NAME    =		palindrome

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -Iinclude -Wall

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
