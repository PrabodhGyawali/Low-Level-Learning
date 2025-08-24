CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -Iinclude

build: trivium_test

trivium_test: trivium.o trivium_test.o
	$(CC) $(CFLAGS) -o trivium_test trivium.o trivium_test.o

trivium.o: trivium.c include/trivium.h
	$(CC) $(CFLAGS) -c trivium.c -o trivium.o

trivium_test.o: test/trivium_test.c include/trivium.h
	$(CC) $(CFLAGS) -c test/trivium_test.c -o trivium_test.o

clean:
	rm -f *.o trivium_test