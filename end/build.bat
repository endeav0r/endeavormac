gcc -Wall -O2 -c src/lexer.c -o obj/lexer.o
gcc -Wall -O2 -c src/number.c -o obj/number.o
gcc -Wall -O2 -c src/symbol.c -o obj/symbol.o
gcc -Wall -O2 -c src/string.c -o obj/string.o
gcc -Wall -O2 -c src/vm.c -o obj/vm.o
gcc -Wall -O2 -c src/end.c -o obj/end.o
gcc -Wall -O2 -c src/function.c -o obj/function.o

gcc -Wall -O2 src/main.c obj/function.o obj/end.o obj/lexer.o obj/number.o obj/symbol.o obj/string.o obj/vm.o -o end.exe