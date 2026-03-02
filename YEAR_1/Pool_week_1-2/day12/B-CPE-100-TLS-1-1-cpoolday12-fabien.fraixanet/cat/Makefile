##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## makefile for cat
##

SRC	=	cat.c

OBJ	=	$(SRC:.c=.o)

NAME	=	cat

CFLAGS	=	-I ../include -L ../lib/ -lmy

all:	$(NAME)

$(NAME):
	make -C ../lib/my
	gcc $(SRC) -o $(NAME) $(CFLAGS)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
