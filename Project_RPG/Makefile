##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## call to Makefile in lib
##

SRC	=	src/main.c	\
		lib/my/my_putchar.c	\
		src/gen_background.c	\
		src/init_view.c	\
		src/init_structs.c	\
		src/create_sprite.c	\
		src/create_hitbox.c	\
		src/player/mooves.c	\
		src/player/player.c	\
		src/map/init_map.c	\
		src/map/walls_tab.c	\
		src/player/collision.c	\
		src/inventory/init_inventory.c	\
		src/inventory/inventory.c	\
		src/createsprite_array.c	\
		src/sprites_tab.c	\
		src/enemy/init_enemies.c	\
		src/start.c	\
		src/player/direction.c	\


OBJ	=	$(SRC:.c=.o)

NAME    =	my_rpg

CSFML	=	-lcsfml-graphics -lcsfml-window -lcsfml-audio -lcsfml-system

MATH	=	-lm

INCL	=	-Iinclude

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib/my
	gcc $(OBJ) -o $(NAME) $(CSFML) $(MATH) $(INCL) lib/my/libmy.a -g -g3 -Wall

clean:
		make -C lib/my/ clean
	rm -f $(OBJ)
	rm -f *~
	rm -f #*#

fclean:	clean
	make -C lib/my/ fclean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
