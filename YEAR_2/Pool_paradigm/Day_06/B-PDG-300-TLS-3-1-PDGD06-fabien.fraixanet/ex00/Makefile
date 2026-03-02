##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	Z.cpp\

NAME	=	Z\

CC	= g++

CFLAGS = -std=c++20 -Wall -Wextra -Werror

all:	$(NAME)

$(NAME):	$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)
clean:
	rm -f *.o

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
