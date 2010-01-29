echo "polarssl/sha1.c"
gcc -O3 -g -pg -c -Wall src/polarssl/sha1.c -o o/sha1.o
echo "polarssl/md5.c"

gcc -O3 -g -pg -c -Wall src/polarssl/md5.c -o o/md5.o
echo "utility.c"
gcc -O2 -g -pg -c -Wall src/utility.c -o o/utility.o
echo "nsrl.c"
gcc -O2 -g -pg -c -Wall src/nsrl.c -o o/nsrl.o
echo "fileinfo.c"
gcc -O2 -g -pg -c -Wall src/fileinfo.c -o o/fileinfo.o
echo "error.c"
gcc -O2 -g -pg -c -Wall src/error.c -o o/error.o

echo "test_fileinfo"
gcc -O2 -g -pg -Wall src/test_fileinfo.c -o test_fileinfo o/fileinfo.o o/utility.o o/sha1.o o/md5.o
echo "test_nsrlfile"
gcc -O2 -g -pg -Wall src/test_nsrlfile.c -o test_nsrlfile o/utility.o o/nsrl.o o/error.o
echo "scan"
gcc -O2 -g -pg -Wall src/scan.c -o scan o/fileinfo.o o/utility.o o/sha1.o o/md5.o `pkg-config --static --libs sqlite3`
echo "nsrlfile"
gcc -O2 -g -pg -Wall src/nsrlfile.c -o nsrlfile o/nsrl.o o/utility.o o/error.o `pkg-config --static --libs sqlite3`
