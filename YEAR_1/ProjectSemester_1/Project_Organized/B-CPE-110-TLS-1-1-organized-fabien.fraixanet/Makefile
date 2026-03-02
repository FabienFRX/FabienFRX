##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	main.c	\
		add.c	\
		del.c	\
		sort.c	\
		disp.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =	organized

INCL	=	-I include/

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) -Wall $(INCL) -Ll ./libmy.a -Ll ./libshell.a

clean:
	rm -f $(OBJ)
	make clean -C lib/my

fclean:	clean
	rm -f $(NAME)
	make fclean -C lib/my

re:	fclean all
