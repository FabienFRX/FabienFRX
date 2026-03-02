##
## EPITECH PROJECT, 2024
## B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	src/main.c					\
		src/display_help.c			\
		src/option_i.c				\
		src/option_e.c				\
		src/option_json.c			\
		src/file_func.c				\
		src/function_base.c			\
		src/my_putstr.c				\
		src/my_strcmp.c				\
		src/my_strncmp.c			\
		src/my_str_to_word_array.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =       SBMLparser

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -Iinclude -Wall

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
