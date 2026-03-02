##
## EPITECH PROJECT, 2025
## MY_FTP
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

SRCDIR     = src

## -------- SRC -------##

SRC     = $(SRCDIR)/Main.cpp				\
		$(SRCDIR)/Server.cpp				\
		$(SRCDIR)/ClientHandler.cpp		\
		$(SRCDIR)/CommandParser.cpp		\
		$(SRCDIR)/DataTransfer.cpp		\

OBJ     = $(SRC:.cpp=.o)

## ------- FLAGS --------##

cxxflags.common := -Wall -Wextra -Werror -std=c++20
cxxflags.debug  := -g
cxxflags.release := -O2
cxxflags.tests  := -fsanitize=address -g

ldflags.common  :=
ldflags.debug   :=
ldflags.release :=
ldflags.tests   :=

CXXFLAGS := ${cxxflags.${BUILD}} ${cxxflags.common}
LDFLAGS  := ${ldflags.${BUILD}} ${ldflags.common}

## ------- BIN ----------##

BINNAME = myftp

## ------- BUILD ----------##

BUILD   = release

## --------- RULES --------##

all: $(BINNAME)
	@$(ECHO) $(GREEN) "[OK]" $(TEAL) " Done : " $@ $(DEFAULT) || \
	$(ECHO) $(ERROR) "[ERROR]" $(YELLOW) $(BINNAME) $(DEFAULT)

$(BINNAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $(BINNAME) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ && \
	$(ECHO) $(GREEN) "[OK] " $(DEFAULT) $(TEAL) $< $(DEFAULT) " -----> " \
    $(GREEN) $@ $(DEFAULT) || \
    $(ECHO) $(ERROR) " [ERROR] can't compile " $(YELLOW) $^ $(DEFAULT)

clean:
	@rm -rf $(OBJ) && \
	$(ECHO) $(MAGENTA) "\tRemoved object files" $(DEFAULT)

fclean: clean
	@rm -rf $(BINNAME) && \
	$(ECHO) $(GREEN) "[OK]" $(TEAL) " Done : " $@ $(DEFAULT) || \
	$(ECHO) $(ERROR) "[ERROR]" $(YELLOW) $(BINNAME) $(DEFAULT)

re: fclean all
