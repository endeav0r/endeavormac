gcc -Wall -g -c src/lexer.c -o obj/lexer.o -DLEXER_DEBUG
gcc -Wall -g -c src/number.c -o obj/number.o -DNUMBER_DEBUG
gcc -Wall -g -c src/symbol.c -o obj/symbol.o -DSYMBOL_DEBUG
gcc -Wall -g -c src/string.c -o obj/string.o -DSTRING_DEBUG
gcc -Wall -g -c src/vm.c -o obj/vm.o -DVM_DEBUG
gcc -Wall -g -c src/end.c -o obj/end.o -DEND_DEBUG
gcc -Wall -g -c src/function.c -o obj/function.o -DFUNCTION_DEBUG

gcc -Wall -g src/main.c obj/function.o obj/end.o obj/lexer.o obj/number.o obj/string.o obj/symbol.o obj/vm.o -o end_debug.exe