gcc -Wall -O3 -c src/lexer.c -o obj/lexer.o
gcc -Wall -O3 -c src/number.c -o obj/number.o
gcc -Wall -O3 -c src/symbol.c -o obj/symbol.o
gcc -Wall -O3 -c src/string.c -o obj/string.o
gcc -Wall -O3 -c src/vm.c -o obj/vm.o
gcc -Wall -O3 -c src/end.c -o obj/end.o

gcc -Wall -g src/debug.c obj/end.o obj/lexer.o obj/number.o obj/symbol.o obj/string.o obj/vm.o -o debug.exe
gcc -Wall -g src/lexer_test.c obj/lexer.o -o lexer_test.exe
gcc -Wall -O3 src/main.c obj/end.o obj/lexer.o obj/number.o obj/symbol.o obj/string.o obj/vm.o -o end.exe

gcc -Wall -g -c src/lexer.c -o obj/lexer.o -DLEXER_DEBUG
gcc -Wall -g -c src/number.c -o obj/number.o -DNUMBER_DEBUG
gcc -Wall -g -c src/symbol.c -o obj/symbol.o -DSYMBOL_DEBUG
gcc -Wall -g -c src/string.c -o obj/string.o -DSTRING_DEBUG
gcc -Wall -g -c src/vm.c -o obj/vm.o -DVM_DEBUG
gcc -Wall -g -c src/end.c -o obj/end.o -DEND_DEBUG

gcc -Wall -g src/main.c obj/end.o obj/lexer.o obj/number.o obj/string.o obj/symbol.o obj/vm.o -o end_debug.exe