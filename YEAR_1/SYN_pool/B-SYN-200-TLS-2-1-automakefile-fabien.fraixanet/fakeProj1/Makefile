CC=clang
CFLAGS=-ftest-coverage -O2
LDFLAGS=
EXEC=fakeProj

SOURCES_DIR=.
LIBS_DIR=librarys
PROJECT_DIR=fakeProj1

SOURCES= ./calc.c ./main.c
OBJECTS=$(SOURCES:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(CFLAGS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f *.gcno

fclean: clean
	rm -f $(EXEC)

re: fclean all

archive:
	mkdir -p $(PROJECT_DIR)/backup
	tar -cvvf $(PROJECT_DIR)/backup/backup_22-06-2024_21h-06m.zip $(SOURCES)
	echo 'backup_22-06-2024_21h-06m.zip file generated.'

revert:
	tar -xvf $(PROJECT_DIR)/backup/backup_22-06-2024_21h-06m.zip

num:
	ls -1 $(PROJECT_DIR)/backup | wc -l

delete:
	rm -f $(PROJECT_DIR)/backup/backup_22-06-2024_21h-06m.zip

.PHONY: all clean fclean re archive revert num delete

