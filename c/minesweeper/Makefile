CC = gcc
CFLAGS = -Wall -pedantic-errors -std=c99 -g
TARGET = minesweeper

all: $(TARGET)

$(TARGET): $(TARGET).c 
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET).exe $(TARGET)
