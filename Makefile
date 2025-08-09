CC=gcc
INC_DIR=-I./include
LIB_DIR=-L/usr/local/lib
LIB=-lglfw3
CFLAGS=-Wall -O2

build:
	$(CC) $(CFLAGS) -o main src/main.c src/glad.c src/file.c $(INC_DIR) $(LIB_DIR) $(LIB)

debug:
	$(CC) $(CFLAGS) -std=c99 -g -o main src/main.c src/glad.c src/file.c $(INC_DIR) $(LIB_DIR) $(LIB)

run:
	./main

quarantine:
	sudo xattr -r -d com.apple.quarantine /usr/local/lib/*.dylib