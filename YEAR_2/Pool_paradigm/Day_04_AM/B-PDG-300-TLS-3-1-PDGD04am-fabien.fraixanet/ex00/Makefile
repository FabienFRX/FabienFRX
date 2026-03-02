##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGRUSH1-fabien.fraixanet
## File description:
## Makefile
##


SRC	=	z.c\

NAME	=	z\

CC	= gcc

CFLAGS = -std=gnu17 -Wall -Wextra

all:	$(NAME)

$(NAME):	$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)
clean:
	rm -f *.o

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
