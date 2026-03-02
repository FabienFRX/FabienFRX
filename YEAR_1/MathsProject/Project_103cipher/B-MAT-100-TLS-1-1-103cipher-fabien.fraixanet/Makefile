##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	main.c			\
		display.c		\
		encryption.c	\
		1_size.c 		\


OBJ	=	$(SRC:.c=.o)

NAME    =	103cipher

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -Iinclude -Wall -Wextra -lm

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
