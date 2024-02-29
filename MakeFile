CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR = src
BUILD_DIR = build

# List all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Derive object file names from source files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Define targets
TARGET = turing
PARSER_TARGET = turingparser

# Phony targets
.PHONY: all clean

all: $(BUILD_DIR) $(TARGET) $(PARSER_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(PARSER_TARGET): $(BUILD_DIR)/$(PARSER_TARGET).o $(BUILD_DIR)/error.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(PARSER_TARGET)
