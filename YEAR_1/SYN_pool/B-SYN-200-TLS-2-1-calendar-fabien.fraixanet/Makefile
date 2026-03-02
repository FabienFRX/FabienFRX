##
## EPITECH PROJECT, 2024
## B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
## File description:
## Makefile
##

SRC	=	src/main.c					\
		src/loop.c					\
		src/manage_employee.c		\
		src/manage_meeting.c		\
		src/command.c				\
		src/command_sort.c			\
		src/display_help.c			\
		src/function_base.c			\
		src/function_base_error.c	\
		src/my_str_to_word_array.c	\

OBJ	=	$(SRC:.c=.o)

NAME	=		calendar

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME) -I./include/ -Wall

%.o:	%.c
	gcc -I./include/ -Wall  -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
