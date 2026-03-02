##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	string.c\
		assign.c\
		append.c\
		at.c\
		clear.c\
		length.c\
		compare.c\
		copy.c\
		c_str.c\
		empty.c\
		find.c\
		insert.c\
		to_int.c\

OBJ	=	$(SRC:.c=.o)

NAME	=	libstring.a\

CFLAGS = -std=gnu17 -Wall -Wextra

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rc $(NAME) $(OBJ)
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
