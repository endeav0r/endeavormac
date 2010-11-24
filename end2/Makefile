C_FLAGS = -c -Wall -g
CC = gcc

OBJ = obj/
SRC = src/

end2 : lexer parser variable vm
	$(CC) -o e2 $(SRC)end2.c $(OBJ)lexer.o $(OBJ)parser.o $(OBJ)variable.o $(OBJ)vm.o

lexer : $(SRC)lexer.c
	$(CC) $(C_FLAGS) $(SRC)lexer.c -o $(OBJ)lexer.o

parser : $(SRC)parser.c
	$(CC) $(C_FLAGS) $(SRC)parser.c -o $(OBJ)parser.o

variable : $(SRC)variable.c
	$(CC) $(C_FLAGS) $(SRC)variable.c -o $(OBJ)variable.o

vm : $(SRC)vm.c
	$(CC) $(C_FLAGS) $(SRC)vm.c -o $(OBJ)vm.o
