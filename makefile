#
# Makefile for csci4401-pa1
#
# Author: Ted Mader
# Date: 2016-09-14
#

CC = gcc
TARGETS = main.exe

all: $(TARGETS)

main.exe: main.o
	$(CC) -o main.exe main.o

main.o: main.c
	$(CC) -c main.c

clean:
	rm -rf *.o main.c
