##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	SRC/ht_search.c				\
		SRC/ht_dump.c				\
		SRC/hash.c					\
		SRC/new_hashtable.c			\
		SRC/delete_hashtable.c		\
		SRC/insert.c				\
		SRC/delete.c				\
		lib/my/my_printf.c			\
		lib/my/my_strdup.c			\
		lib/my/my_strcpy.c			\
		lib/my/my_strlen.c			\
		lib/my/my_put_nbr_base.c	\
		lib/my/my_put_nbr.c			\
		lib/my/my_put_nbr_u.c		\
		lib/my/my_putchar.c			\
		lib/my/my_put_nbr_base_low.c\
		lib/my/my_put_nbr_base_cap.c\
		lib/my/my_put_nbr_display.c	\
		lib/my/my_put_nbr_hexa.c	\
		lib/my/my_putstr.c			\
		lib/my/print_p.c			\
		lib/my/my_print_float.c		\
		lib/my/my_print_e_low.c		\
		lib/my/my_print_e_cap.c		\


SRC_main =	SRC/main.c			\

OBJ	=	$(SRC:.c=.o)

OBJ_main =	$(SRC_main: .c=.o)

NAME    =	libhashtable.a

TESTNAME	= secured

INCL	=	-I include/

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rc $(NAME) $(OBJ)

%.o:	%.c
	gcc -c $< -o $@ -Wall $(INCL)

test:	$(TESTNAME)

$(TESTNAME):	$(NAME)
	make -C lib/my
	gcc $(OBJ_main) -Ll ./$(NAME) -o $(TESTNAME) -Wall $(INCL) -Ll ./libmy.a

clean:
	rm -f $(OBJ)
	make clean -C lib/my

fclean:	clean
	rm -f $(NAME)
	rm -f $(TESTNAME)
	make fclean -C lib/my

re:	fclean all
