CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE -Isrc

TARGET = myalloc_test

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) tests/*.o src/*.o

rebuild: clean all

.PHONY: all run clean rebuild