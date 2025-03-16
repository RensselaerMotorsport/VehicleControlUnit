CC = gcc
CFLAGS = -g -O0 -I../Inc -pthread -lm -MMD

OBJ_DIR = Obj

VCU_DIR = ../Src
VCU_SRCS = $(shell find $(VCU_DIR) -type f -name '*.c' \
          ! -name 'main.c' \
          ! -name 'stm32*.c' \
          ! -name '*hal*.c' \
          ! -name '*HAL*.c' \
          ! -name 'syscalls*.c' \
          ! -name 'sysmem*.c' \
          ! -name 'system_stm32*.c' \
          ! -name 'startup_stm32*.c' \
          ! -path '*/STM32*/*' \
          ! -path '*/Drivers/*')
VCU_OBJ_DIR = ../Obj
VCU_OBJS = $(VCU_SRCS:$(VCU_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_DIR = .
TEST_SRCS = $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJ_DIR = $(OBJ_DIR)/Test
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
CFLAGS += -DTEST_MODE -IInc/fake_include

TEST_EXECUTABLE = test

all: debug $(TEST_EXECUTABLE)

debug:
	@echo "VCU_SRCS = $(VCU_SRCS)"
	@echo "VCU_OBJS = $(VCU_OBJS)"
	@echo "TEST_SRCS = $(TEST_SRCS)"
	@echo "TEST_OBJS = $(TEST_OBJS)"

$(TEST_EXECUTABLE): $(VCU_OBJS) $(TEST_OBJS)
	@echo "TODO LDFLAGS?"
	$(CC) $(CFLAGS) $(VCU_OBJS) $(TEST_OBJS) -o $@
	@echo "Built main test executable $@"

$(OBJ_DIR)/%.o: $(VCU_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) main.out $(shell find $(TEST_DIR) -type f -name '*.out')
	@echo "Cleaned up build files."
