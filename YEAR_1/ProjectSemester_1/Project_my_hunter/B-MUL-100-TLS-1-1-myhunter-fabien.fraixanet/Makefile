##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	main.c			\
		all_init.c		\
		music.c			\
		all_texture.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =	my_hunter

INCL	=	-Iinclude/

CSFML	=	-lcsfml-graphics -lcsfml-window -lcsfml-audio -lcsfml-system

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) $(INCL) $(CSFML) -Wall
clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)
re:	fclean all
