##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	src/main.c					\
		src/parsing.c				\
		src/setenv.c				\
		src/env.c					\
		src/cd.c					\
		src/ls.c					\

OBJ	=	$(SRC:.c=.o)

NAME    =       mysh

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) -Iinclude libmy.a -Wall

clean:
	rm -f $(OBJ)
	make clean -C lib/my

fclean:	clean
	rm -f $(NAME)
	make fclean -C lib/my

re:	fclean all
