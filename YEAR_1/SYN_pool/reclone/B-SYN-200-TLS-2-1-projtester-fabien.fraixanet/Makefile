##
## EPITECH PROJECT, 2024
## B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	src/main.c					\
		src/display_func.c			\
		src/function_base.c			\
		src/tree_handle.c			\
		src/command_handle.c		\
		src/sort_tree.c				\
		src/my_str_to_word_array.c	\
		src/free.c					\


OBJ	=	$(SRC:.c=.o)

NAME    =		projTester

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -Iinclude -Wall -Wextra

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
