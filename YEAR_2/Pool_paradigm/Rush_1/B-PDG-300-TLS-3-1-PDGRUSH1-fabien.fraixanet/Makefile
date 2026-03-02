##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGRUSH1-fabien.fraixanet
## File description:
## Makefile
##


SRC	=	pushswap_checker.hs \

NAME	=	pushswap_checker \

HC	= ghc

all:	$(NAME)

$(NAME):	$(SRC)
	$(HC) $(SRC) -o $(NAME)

clean:
	rm -f *.hi *.o

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
