# Compiler settings
CC = gcc
# TODO: Remove -g and -O0 flags for production
CFLAGS = -g -O0 -IIncludes -pthread -lm -DTEST

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

# Test objects compilation
TEST_SOURCES = $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

all: main.out

main.out: $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS)

# Generic rule for compiling C source to object files in SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Generic rule for compiling C source to object files in TEST_DIR
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Rule for compiling and linking test executables
$(TEST_DIR)/%.out: $(TEST_DIR)/%.c $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $< $(OBJECTS) -o $@ $(CFLAGS)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) *.out

# Include dependencies
-include $(OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)
