CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2
SRC = main.c dijkstra.c graph.c util.c
HEADER = dijkstra.h graph.h util.h

all: dijkstra

dijkstra: $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -o dijkstra -lm

clean:
	rm -f dijkstra

.PHONY: all clean
