# Compiler and flags
CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Iinclude -MMD -MP
LDFLAGS  := -pthread

# Directories
SRC_DIR  := src
TEST_DIR := tests
OBJ_DIR  := build
BIN      := 3d-renderer
TEST_BIN := test-runner

# Source files and object files
SRCS     := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c') # Exclude main.c for tests
OBJS     := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS     := $(OBJS:.o=.d)

TEST_SRCS := $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)

# Default target: build the main program
all: $(BIN)

# Build the main program
$(BIN): $(OBJS) $(OBJ_DIR)/main.o
	@echo "Linking $@"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build the test runner
$(TEST_BIN): $(OBJS) $(TEST_OBJS)
	@echo "Compiling tests..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files to object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
.PHONY: check
check: $(TEST_BIN)
	@echo "Running tests..."
	./$(TEST_BIN)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning up!"
	rm -rf $(OBJ_DIR) $(BIN) $(TEST_BIN)

# Include dependency files
-include $(DEPS) $(TEST_DEPS)