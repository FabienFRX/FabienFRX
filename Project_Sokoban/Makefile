##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	SRC/main.c				\
		SRC/error_handling.c	\
		SRC/direction_hor.c		\
		SRC/direction_ver.c		\
		SRC/disp_help.c			\
		SRC/wincond.c			\

OBJ	=	$(SRC:.c=.o)

NAME    =       my_sokoban

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) -lncurses -Iinclude libmy.a -Wall
clean:
	rm -f $(OBJ)
	make clean -C lib/my

fclean:	clean
	rm -f $(NAME)
	make fclean -C lib/my

re:	fclean all
