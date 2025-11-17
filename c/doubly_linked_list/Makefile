CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = doubly_linked_list.c
HEADER = doubly_linked_list.h

all: dll_program test_dll

dll_program: $(SRC) main.c $(HEADER)
	$(CC) $(CFLAGS) $(SRC) main.c -o dll_program

test_dll: $(SRC) test_doubly_linked_list.c $(HEADER)
	$(CC) $(CFLAGS) $(SRC) test_doubly_linked_list.c -o test_dll

test: test_dll
	./test_dll

run: dll_program
	./dll_program

clean:
	rm -f dll_program test_dll *.o

.PHONY: all test run clean
