##
## EPITECH PROJECT, 2018
## Makefile
## File description:
## make of the minishell2
##

NAME	= 	mysh

CC		= 	gcc

FOLDER	=	src

SRC	=		$(FOLDER)/my_strncmp.c			\
			$(FOLDER)/my_strcmp.c			\
			$(FOLDER)/my_strlen.c			\
			$(FOLDER)/my_strlen_array.c		\
			$(FOLDER)/my_getenv.c			\
			$(FOLDER)/my_unsetenv.c			\
			$(FOLDER)/my_setenv.c			\
			$(FOLDER)/my_calloc_ar.c		\
			$(FOLDER)/my_calloc_str.c		\
			$(FOLDER)/my_strdup.c			\
			$(FOLDER)/my_tabdup.c			\
			$(FOLDER)/my_putstr.c			\
			$(FOLDER)/my_env.c				\
			$(FOLDER)/single_old.c			\
			$(FOLDER)/my_builtin_cd.c		\
			$(FOLDER)/my_strcat.c			\
			$(FOLDER)/my_strcat_path.c		\
			$(FOLDER)/my_access.c			\
			$(FOLDER)/my_cmd.c				\
			$(FOLDER)/clean_string.c		\
			$(FOLDER)/my_pipe.c				\
			$(FOLDER)/is_builtin.c			\
			$(FOLDER)/my_error.c			\
			$(FOLDER)/my_free.c				\
			$(FOLDER)/my_exit_cmd.c			\
			$(FOLDER)/my_input.c			\
			$(FOLDER)/write_my_error.c		\
			$(FOLDER)/my_strtab.c			\
			$(FOLDER)/single_env.c			\
			$(FOLDER)/handling_cmd.c		\
			$(FOLDER)/replace_char.c		\
			$(FOLDER)/main.c

CFLAGS	=	-I./include -Wall -Wextra

OBJ	=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -o $(NAME)

clean:
			rm -rf $(OBJ)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY: all clean fclean re
