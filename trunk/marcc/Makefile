FLAGS = -c -Wall -g
CPP = g++

BUILD_DIR = obj/
SRC_DIR = src/

marcc : exception instruction lexer parser parserstack symboltable token
	$(CPP) -Wall -g -o marcc $(SRC_DIR)marcc.cpp \
	$(BUILD_DIR)exception.o $(BUILD_DIR)instruction.o $(BUILD_DIR)lexer.o $(BUILD_DIR)parser.o \
	$(BUILD_DIR)parserstack.o $(BUILD_DIR)symboltable.o $(BUILD_DIR)token.o

exception : $(SRC_DIR)exception.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)exception.cpp -o $(BUILD_DIR)exception.o

instruction : $(SRC_DIR)instruction.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)instruction.cpp -o $(BUILD_DIR)instruction.o

lexer : $(SRC_DIR)lexer.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)lexer.cpp -o $(BUILD_DIR)lexer.o

parser : $(SRC_DIR)parser.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)parser.cpp -o $(BUILD_DIR)parser.o

parserstack : $(SRC_DIR)parserstack.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)parserstack.cpp -o $(BUILD_DIR)parserstack.o

symboltable : $(SRC_DIR)symboltable.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)symboltable.cpp -o $(BUILD_DIR)symboltable.o

token : $(SRC_DIR)token.cpp
	$(CPP) $(FLAGS) $(SRC_DIR)token.cpp -o $(BUILD_DIR)token.o
