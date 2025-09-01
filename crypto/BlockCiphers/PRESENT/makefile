CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -Iinclude

build: present_test

present_test: present.o present_test.o
	$(CC) $(CFLAGS) -o present_test present.o present_test.o

present.o: present.c include/present.h
	$(CC) $(CFLAGS) -c present.c -o present.o

present_test.o: test/present_test.c include/present.h
	$(CC) $(CFLAGS) -c test/present_test.c -o present_test.o

clean:
	rm -f *.o present_test