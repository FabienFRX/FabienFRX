##
## EPITECH PROJECT, 2023
## makefile
## File description:
## first makefile
##

SRC     =       rush3.c	\
		my_putchar.c	\
		my_put_nbr.c	

OBJ     =	$(SRC:.c=.o)

NAME    =	stumper

CFLAGS  =	-I ../include -L


all:	$(NAME)

$(NAME):
	make -C /lib/my
	gcc $(SRC) -o $(NAME) $(CFLAGS)
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re: fclean all
