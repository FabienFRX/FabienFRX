##
## EPITECH PROJECT, 2023
## makefile
## File description:
## first makefile
##

SRC	=	rush3.c		\
		my_putchar.c	\
		my_put_nbr.c

OBJ	=	$(SRC:.c=.o)

NAME	=	libmy.a

CFLAGS	=	-I./include -Werror -Wall -Wextra

all:	$(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	cp my.h ../../include/my.h
	cp $(NAME) ../

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f ../$(NAME)

re: fclean all
