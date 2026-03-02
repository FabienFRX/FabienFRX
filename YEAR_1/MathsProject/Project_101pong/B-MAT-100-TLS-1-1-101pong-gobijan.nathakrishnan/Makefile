##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile 101pong
##

SRC	=	101pong.c	\

NAME	=	101pong

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)


$(NAME):
	gcc $(SRC) -o $(NAME) -lm

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(NAME)

re: fclean all
