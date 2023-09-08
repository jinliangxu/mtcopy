mtcopy
======

Introduction:
multi-thread file copy program for testing.

Usage:
1. compile
  g++ -o mtcopy mtcopy.cpp -lpthread
  g++ -o mtcopy mtcopy.cpp -pthread

  For debug logs plase add "-D_DEBUG" to compiler options. For example,
  g++ -D_DEBUG -o mtcopy mtcopy.cpp -lpthread

2. execute
  ./mtcopy <source file> <dest file> <num of threads>

Copyrights:
Testing code for multi-thread study and github usage.
