##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	main.c		\
		display.c	\
		init_tow.c	\
		init_pla.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =	my_radar

CSFML	=	-lcsfml-graphics -lcsfml-window -lcsfml-audio -lcsfml-system

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) $(CSFML) -Iinclude libmy.a -Wall
clean:
	rm -f $(OBJ)
	make clean -C lib/my

fclean:	clean
	rm -f $(NAME)
	make fclean -C lib/my

re:	fclean all
