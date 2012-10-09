# Makefile

TARGET := threadingExample
CFLAGS := -Wall -g -lpthread
CC := g++

all:: x${TARGET}.o

x${TARGET}.o: ${TARGET}.cpp
	$(CC) $(CFLAGS) ${INCLUDE} ${TARGET}.cpp -o x${TARGET}.o

