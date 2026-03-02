##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	main.c	\
		display_map.c	\
		my_str_to_word_array.c	\
		replace_square.c	\

OBJ	=	$(SRC:.c=.o)

NAME    =	setting_up

INCL	=	-I include/ -L lib/ -lmy

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) -Iinclude libmy.a -Wall
clean:
	rm -f $(OBJ)
	make clean -C lib/my
























fclean:	clean
	rm -f $(NAME)
	make fclean -C lib/my

re:	fclean all
