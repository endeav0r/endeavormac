FLAGS = -c -Wall -g
CPP = g++

BUILD_DIR = obj/
SRC_DIR = src/

marcc : astree compiler exception instruction lexer parser register symbol \
        symboltable token labelmaker memorydefinition
	$(CPP) -Wall -g -o marcc $(SRC_DIR)marcc.cpp $(BUILD_DIR)compiler.o $(BUILD_DIR)labelmaker.o \
	$(BUILD_DIR)exception.o $(BUILD_DIR)instruction.o $(BUILD_DIR)lexer.o $(BUILD_DIR)parser.o \
	$(BUILD_DIR)astree.o $(BUILD_DIR)symboltable.o $(BUILD_DIR)token.o $(BUILD_DIR)symbol.o \
	$(BUILD_DIR)register.o $(BUILD_DIR)memorydefinition.o


compiler : $(SRC_DIR)compiler.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)compiler.cpp -o $(BUILD_DIR)compiler.o
	
astree : $(SRC_DIR)astree.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)astree.cpp -o $(BUILD_DIR)astree.o

exception : $(SRC_DIR)exception.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)exception.cpp -o $(BUILD_DIR)exception.o

instruction : $(SRC_DIR)instruction.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)instruction.cpp -o $(BUILD_DIR)instruction.o

labelmaker : $(SRC_DIR)labelmaker.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)labelmaker.cpp -o $(BUILD_DIR)labelmaker.o

lexer : $(SRC_DIR)lexer.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)lexer.cpp -o $(BUILD_DIR)lexer.o

memorydefinition : $(SRC_DIR)memorydefinition.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)memorydefinition.cpp -o $(BUILD_DIR)memorydefinition.o

parser : $(SRC_DIR)parser.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)parser.cpp -o $(BUILD_DIR)parser.o

register : $(SRC_DIR)register.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)register.cpp -o $(BUILD_DIR)register.o

symbol : $(SRC_DIR)symbol.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)symbol.cpp -o $(BUILD_DIR)symbol.o

symboltable : $(SRC_DIR)symboltable.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)symboltable.cpp -o $(BUILD_DIR)symboltable.o

token : $(SRC_DIR)token.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)token.cpp -o $(BUILD_DIR)token.o
