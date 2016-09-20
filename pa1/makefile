#
# Makefile for csci4401-pa1
#
# Author: Ted Mader
# Date: 2016-09-14
#

CC = gcc
TARGETS = mash.exe

all: $(TARGETS)

mash.exe: commands.o main.o
	$(CC) -o mash.exe commands.o main.o

commands.o: commands.c
	$(CC) -c commands.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -rf *.o *.txt mash.exe
