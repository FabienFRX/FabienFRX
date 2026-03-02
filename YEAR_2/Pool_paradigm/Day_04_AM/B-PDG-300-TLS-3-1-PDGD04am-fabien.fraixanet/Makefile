##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGRUSH1-fabien.fraixanet
## File description:
## Makefile
##


SRC	=	main.c\

NAME	=	day04 \

CC	= gcc

all:	$(NAME)

$(NAME):	$(SRC)
	$(CC) $(SRC) -o $(NAME) -Wall -Wextra

clean:
	rm -f *.o

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
