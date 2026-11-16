CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -Iinclude
SRC = main.c dijkstra.c graph.c util.c
HEADER = include/dijkstra.h include/graph.h include/util.h

all: dijkstra

dijkstra: $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -o dijkstra -lm

clean:
	rm -f dijkstra

.PHONY: all clean
