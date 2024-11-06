# Compiler settings
CC = gcc
# TODO: Remove -g and -O0 flags for production
CFLAGS = -g -O0 -IIncludes -pthread -lm

# Base source and test directories
SRC_DIR = Src
TEST_DIR = Tests

# Object files directory
OBJ_DIR = Obj

# Recursively find all C sources in subdirectories of SRC_DIR,
# excluding the SRC_DIR root itself
SOURCES = $(shell find $(SRC_DIR) -mindepth 2 -type f -name '*.c')
# Create corresponding object files paths
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test sources and objects
TEST_SOURCES = $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Check if TEST_MODE is set
ifdef TEST_MODE
    CFLAGS += -DTEST_MODE
endif

# Default target
all: prepare main.out

# Prepare directory structure
prepare:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)
	@echo "Directory structure created."

# Main program build
main.out: $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS)
	@echo "Built main program: $@"

# Rule for compiling C source to object files in SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Rule for compiling C source to object files in TEST_DIR
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Rule for compiling and linking test executables
$(TEST_DIR)/%.out: $(TEST_DIR)/%.c $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $< $(OBJECTS) -o $@ $(CFLAGS)
	@echo "Built test executable: $@"

# Clean rule
clean:
	rm -rf $(OBJ_DIR) *.out
	@echo "Cleaned up build files."

# Include dependencies
-include $(OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)
