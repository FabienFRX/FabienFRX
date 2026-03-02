##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefil root
##

SRC	=	my_printf.c	\

OBJ	=	$(SCR:.c=.o)

NAME	=	my_printf

CFLAGS	=	-I include -L lib/ -lmy

all:	$(NAME)

$(NAME):
	make -C lib/my
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
