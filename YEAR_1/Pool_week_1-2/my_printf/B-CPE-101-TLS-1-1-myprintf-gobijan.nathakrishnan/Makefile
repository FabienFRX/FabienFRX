##
## EPITECH PROJECT, 2023
## makefile
## File description:
## first makefile
##

SRC	=	
		my_putchar.c	\
		my_put_nbr.c	\
		my_putstr.c	\
		my_strlen.c	\
		my_put_nbr_base.c	\
		my_printf.c	\
		my_put_nbr_u.c	\
		print_p.c	\
		my_print_float.c

NAME	=	my_printf

CFLAGS	=	-I./include -Wall -Werror -Wextra -g3

all:	$(NAME)

$(NAME): clean
	gcc -o $(NAME) $(SRC) -I include -L lib/ -lmy $(CFLAGS)
	make clean

clean:
	make clean -C lib/
	rm -f *.o
	rm -f *.log

fclean: clean
	rm -f $(NAME)

re: fclean all
