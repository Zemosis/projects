CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Iinclude -Ilib -MMD -MP
LDLIBS = -lm
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
DEPS = $(OBJS:.o=.d)

# Records the flags of the last build so switching debug/release rebuilds
FLAGS_FILE = $(BUILD_DIR)/flags

# Default target: debug build
all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(FLAGS_FILE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Only touched when the flags change, so objects rebuild only then
$(FLAGS_FILE): FORCE | $(BUILD_DIR)
	@echo '$(CFLAGS)' | cmp -s - $@ || echo '$(CFLAGS)' > $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET) $(TARGET).exe $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)

FORCE:

-include $(DEPS)

.PHONY: all debug release clean FORCE
