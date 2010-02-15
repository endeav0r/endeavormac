g++ -g -O2 -Wall -c src/map.cpp -o o/map.o
g++ -g -O2 -Wall -c src/exception.cpp -o o/exception.o
g++ -g -O2 -Wall -c src/surface.cpp -o o/surface.o
g++ -g -O2 -Wall -c src/game.cpp -o o/game.o
g++ -g -O2 -Wall -c src/tank.cpp -o o/tank.o
g++ -g -O2 -Wall -c src/main.cpp -o o/main.o

g++ -g -O2 -Wall o/*.o -o game.exe lua51.dll alleg42.dll
