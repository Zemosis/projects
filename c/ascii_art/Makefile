CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2
TARGET = ascii_art

# Source files
SRCS = ascii_art.c image.c
OBJS = $(SRCS:.c=.o)

# Default target: debug build
all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all debug release clean
