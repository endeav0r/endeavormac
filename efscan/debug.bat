gcc -O3 -g -pg -c -Wall src/polarssl/sha1.c -o o/sha1.o
gcc -O3 -g -pg -c -Wall src/polarssl/md5.c -o o/md5.o

gcc -O2 -g -pg -c -Wall src/utility.c -o o/utility.o
gcc -O2 -g -pg -c -Wall src/nsrl.c -o o/nsrl.o
gcc -O2 -g -pg -c -Wall src/fileinfo.c -o o/fileinfo.o
gcc -O2 -g -pg -c -Wall src/error.c -o o/error.o

gcc -O2 -g -pg -Wall src/test_fileinfo.c -o test_fileinfo.exe o/fileinfo.o o/utility.o o/sha1.o o/md5.o
gcc -O2 -g -pg -Wall src/test_nsrlfile.c -o test_nsrlfile.exe o/utility.o o/nsrl.o o/error.o
gcc -O2 -g -pg -Wall src/scan.c -o scan.exe o/fileinfo.o o/utility.o o/sha1.o o/md5.o o/sqlite3-o2.o
