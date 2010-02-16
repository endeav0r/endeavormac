echo "clean"
rm o/*.o
echo "map"
g++ -g -O2 -Wall -c src/map.cpp -o o/map.o -Wno-write-strings
echo "exception"
g++ -g -O2 -Wall -c src/exception.cpp -o o/exception.o
echo "surface"
g++ -g -O2 -Wall -c src/surface.cpp -o o/surface.o -Wno-write-strings
echo "game"
g++ -g -O2 -Wall -c src/game.cpp -o o/game.o -Wno-write-strings
echo "tank"
g++ -g -O2 -Wall -c src/tank.cpp -o o/tank.o -Wno-write-strings
echo "main"
g++ -g -O2 -Wall -c src/main.cpp -o o/main.o -Wno-write-strings
echo "linking..."
g++ -g -O2 -Wall o/map.o o/exception.o o/surface.o o/game.o o/tank.o o/main.o `pkg-config lua5.1 --libs --cflags` `allegro-config --libs --cppflags` -o game
