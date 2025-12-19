CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Iinclude -Ilib -lm
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2
TARGET = ascii_art

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/ascii_art.c $(SRC_DIR)/image.c $(SRC_DIR)/ascii.c
OBJS = $(BUILD_DIR)/ascii_art.o $(BUILD_DIR)/image.o $(BUILD_DIR)/ascii.o

# Default target: debug build
all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET) $(TARGET).exe $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)

.PHONY: all debug release clean
