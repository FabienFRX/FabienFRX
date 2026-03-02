##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	src/main.c					\
		src/step_1.c				\
		src/step_2.c				\
		src/step_3.c				\

OBJ	=	$(SRC:.c=.o)

NAME    =       maze_d

INCL	=	-I include/

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -Iinclude -Wall
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
