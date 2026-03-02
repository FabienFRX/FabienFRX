##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	info_file.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =       info_file

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):
	make -C lib/my

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
