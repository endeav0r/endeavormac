cls
g++ -O2 -Wall -c src/exception.cpp -o obj/exception.o
g++ -O2 -Wall -c src/lexer.cpp -o obj/lexer.o
g++ -O2 -Wall -c src/instruction.cpp -o obj/instruction.o
g++ -O2 -Wall -c src/parser.cpp -o obj/parser.o
g++ -O2 -Wall -c src/parserstack.cpp -o obj/parserstack.o
g++ -O2 -Wall -c src/register.cpp -o obj/register.o
g++ -O2 -Wall -c src/symbol.cpp -o obj/symbol.o
g++ -O2 -Wall -c src/symboltable.cpp -o obj/symboltable.o
g++ -O2 -Wall -c src/token.cpp -o obj/token.o

g++ -Wall -O2 -enable-auto-import src/marcc.cpp -o marcc obj/token.o obj/exception.o obj/lexer.o obj/instruction.o obj/register.o obj/symbol.o obj/symboltable.o obj/parser.o obj/parserstack.o
