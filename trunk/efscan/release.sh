echo "polarssl/sha1.c"
gcc -O3 -c -Wall src/polarssl/sha1.c -o o/sha1.o
echo "polarssl/md5.c"
gcc -O3 -c -Wall src/polarssl/md5.c -o o/md5.o

echo "utility.c"
gcc -O2 -c -Wall src/utility.c -o o/utility.o
echo "nsrl.c"
gcc -O2 -c -Wall src/nsrl.c -o o/nsrl.o
echo "fileinfo.c"
gcc -O2 -c -Wall src/fileinfo.c -o o/fileinfo.o
echo "error.c"
gcc -O2 -c -Wall src/error.c -o o/error.o

echo "scan"
gcc -O2 -Wall src/scan.c -o scan o/fileinfo.o o/utility.o o/sha1.o o/md5.o `pkg-config --static --libs sqlite3`
echo "nsrlfile"
gcc -O2 -Wall src/nsrlfile.c -o nsrlfile o/nsrl.o o/utility.o o/error.o `pkg-config --static --libs sqlite3`
