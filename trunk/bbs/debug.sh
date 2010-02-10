#!/bin/bash

echo "bbs_db"
gcc -O2 -g -c bbs_db.c -o o/bbs_db.o

echo "test_bbs_db"
gcc -O2 -g test_bbs_db.c -o test_bbs_db o/bbs_db.o
