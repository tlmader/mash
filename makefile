#
# Makefile for csci4401-pa1
#
# Author: Ted Mader
# Date: 2016-09-14
#

CC = gcc
TARGETS = mash.exe

all: $(TARGETS)

mash.exe: main.o
	$(CC) -o mash.exe main.o

main.o: main.c
	$(CC) -c main.c

clean:
	rm -rf *.o main.c
