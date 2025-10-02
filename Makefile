CLEANFILES = *.exe *.txt *.o

SRC = main.c
OBJ = $(SRC:.c=.o)
EXE = main.exe

all: $(EXE)
	./$(EXE)

$(EXE): $(OBJ)
	gcc $(OBJ) -o $(EXE)

%.o: %.c
	gcc -c $< -o $@

clear:
	rm -f $(CLEANFILES) || del $(CLEANFILES)