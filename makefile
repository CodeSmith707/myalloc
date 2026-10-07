CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE -Isrc -g -O0

TARGET = myalloc_test

SRC = src/myalloc.c tests/main.c

OBJ = src/myalloc.o tests/main.o


.PHONY: all run valgrind clean rebuild


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


src/myalloc.o: src/myalloc.c src/myalloc.h
	$(CC) $(CFLAGS) -c src/myalloc.c -o src/myalloc.o


tests/main.o: tests/main.c src/myalloc.h
	$(CC) $(CFLAGS) -c tests/main.c -o tests/main.o


run: $(TARGET)
	./$(TARGET)


valgrind: $(TARGET)
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --errors-for-leak-kinds=all \
	         ./$(TARGET)


clean:
	rm -f $(OBJ) $(TARGET)


rebuild: clean all