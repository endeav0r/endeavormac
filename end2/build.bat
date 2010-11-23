gcc -c -Wall -g src/parser.c -o obj/parser.o
gcc -c -Wall -g src/vm.c -o obj/vm.o
gcc -c -Wall -g src/lexer.c -o obj/lexer.o
gcc -c -Wall -g src/variable.c -o obj/variable.o
gcc -Wall -g src/end2.c obj/parser.o obj/vm.o obj/lexer.o obj/variable.o -o end2