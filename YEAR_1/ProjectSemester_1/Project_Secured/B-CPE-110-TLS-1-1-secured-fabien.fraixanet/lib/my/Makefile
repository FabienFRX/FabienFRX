##
## EPITECH PROJECT, 2023
## makefile
## File description:
## first makefile
##

SRC	=	my_compute_power_rec.c	\
		my_compute_square_root.c	\
		my_find_prime_sup.c	\
		my_getnbr.c	\
		my_isneg.c	\
		my_is_prime.c	\
		my_putchar.c	\
		my_put_nbr.c	\
		my_putstr.c	\
		my_revstr.c	\
		my_showmem.c	\
		my_showstr.c	\
		my_sort_int_array.c	\
		my_strcapitalize.c	\
		my_strcat.c	\
		my_strcmp.c	\
		my_strcpy.c	\
		my_str_isalpha.c	\
		my_str_islower.c	\
		my_str_isnum.c	\
		my_str_isprintable.c	\
		my_str_isupper.c	\
		my_strlen.c	\
		my_strlowcase.c	\
		my_strncat.c	\
		my_strncmp.c	\
		my_strncpy.c	\
		my_strstr.c	\
		my_strupcase.c	\
		my_swap.c	\
		my_math.c	\
		my_strdup.c	\
		my_put_nbr_display.c	\
		my_put_nbr_hexa.c	\
		my_put_nbr_base.c	\
		my_put_nbr_base_cap.c	\
		my_put_nbr_base_low.c	\
		my_printf.c	\
		my_put_nbr_u.c	\
		print_p.c	\
		my_print_float.c	\
		my_print_e_low.c	\
		my_print_e_cap.c	\

OBJ	=	$(SRC:.c=.o)

NAME	=	libmy.a

CFLAGS	=	-I./include -Wall -Wextra

all:	$(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	cp $(NAME) ../../

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f ../../$(NAME)

re: fclean all
