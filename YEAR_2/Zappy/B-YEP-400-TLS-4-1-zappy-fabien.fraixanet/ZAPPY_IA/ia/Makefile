##
## EPITECH PROJECT, 2025
## B-YEP-400-TLS-4-1-zappy-romain1.chantavongsa
## File description:
## Makefile
##

SRC	:=	src/main.py \

NAME := ../zappy_ai

all: $(NAME)

$(NAME):
	cp $(SRC) $(NAME)
	chmod 777 $(NAME)

clean:
	rm -f $(NAME)
	rm -f *~
	rm -f vgcore*
	rm -f coding-style-reports.log*
	rm -f ${OBJ}

fclean:
	rm -f $(NAME)

re: clean fclean all

.PHONY: all re fclean clean
