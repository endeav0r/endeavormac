cls
g++ -Wall -c src/exception.cpp -o obj/exception.o
g++ -Wall -c src/lexer.cpp -o obj/lexer.o
g++ -Wall -c src/instruction.cpp -o obj/instruction.o
g++ -Wall -c src/parser.cpp -o obj/parser.o
g++ -Wall -c src/parserstack.cpp -o obj/parserstack.o
g++ -Wall -c src/register.cpp -o obj/register.o
g++ -Wall -c src/symbol.cpp -o obj/symbol.o
g++ -Wall -c src/symboltable.cpp -o obj/symboltable.o
g++ -Wall -c src/token.cpp -o obj/token.o

g++ -Wall -g -enable-auto-import src/marcc.cpp -o marcc obj/token.o obj/exception.o obj/lexer.o obj/instruction.o obj/register.o obj/symbol.o obj/symboltable.o obj/parser.o obj/parserstack.o
