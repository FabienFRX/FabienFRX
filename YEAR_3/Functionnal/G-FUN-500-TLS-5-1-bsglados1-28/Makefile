NAME = glados
SRC = src/Main.hs src/SExpr.hs src/AST.hs src/Eval.hs

all:
	ghc -o $(NAME) $(SRC)

clean:
	rm -f *.hi *.o src/*.hi src/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all
