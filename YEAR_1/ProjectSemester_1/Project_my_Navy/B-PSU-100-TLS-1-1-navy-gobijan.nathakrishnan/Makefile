##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	SRC/connect.c			\
		SRC/players.c			\
		SRC/populate_map.c		\
		SRC/position_checker.c	\
		SRC/game_one.c			\
		SRC/file_reader.c		\
		SRC/error_manage.c		\
		SRC/display_h.c			\
		SRC/main.c				\

OBJ	=	$(SRC:.c=.o)

NAME    =	my_navy

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
