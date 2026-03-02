##
## EPITECH PROJECT, 2025
## Tekspice
## File description:
## Makefile
##

## --------- COLOR ------##

DEFAULT = "\033[00m"
GREEN   = "\033[1;32m"
TEAL    = "\033[1;36m"
YELLOW  = "\033[1;7;25;33m"
MAGENTA = "\033[1;3;4;35m"
ERROR   = "\033[5;7;1;31m"

ECHO    = echo -e

## -------- COMPIL ----##

CXX     = g++

## ------- DIR --------##

SRC_DIR = src

## -------- SRC -------##

SRC =			$(SRC_DIR)/Main.cpp 				\
				$(SRC_DIR)/Parser.cpp 				\
				$(SRC_DIR)/Loop.cpp 				\
				$(SRC_DIR)/Circuit.cpp 				\

## -------- OBJ -------##

OBJ = $(SRC:.cpp=.o)

## ------- FLAGS --------##

cxxflags.common := -Wall -Wextra -Werror -std=c++20
cxxflags.debug  := -g
cxxflags.release := -O2
cxxflags.tests  := -fsanitize=address -g

ldflags.common  :=
ldflags.debug   :=
ldflags.release :=
ldflags.tests   :=

BUILD   ?= release  # Définit release par défaut si non spécifié

CXXFLAGS := ${cxxflags.${BUILD}} ${cxxflags.common}
LDFLAGS  := ${ldflags.${BUILD}} ${ldflags.common}

## ------- BIN ----------##

BINNAME = nanotekspice

## --------- RULES --------##

all: $(BINNAME)
	@$(ECHO) $(GREEN) "[OK]" $(TEAL) " Done : " $@ $(DEFAULT)

$(BINNAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $(BINNAME) $(OBJ) $(LDFLAGS)
	@$(ECHO) $(GREEN) "[OK]" $(TEAL) " Built executable: " $(BINNAME) $(DEFAULT)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(ECHO) $(GREEN) "[OK] " $(DEFAULT) $(TEAL) $<  $(DEFAULT)  " -----> " $(GREEN) $@ $(DEFAULT)

clean:
	@rm -f $(SRC_DIR)/*.o
	@$(ECHO) $(MAGENTA) "\tRemoved object files" $(DEFAULT)

fclean: clean
	@rm -f $(BINNAME)
	@$(ECHO) $(GREEN) "[OK]" $(TEAL) "  Done : " $@ $(DEFAULT)

re: fclean all
