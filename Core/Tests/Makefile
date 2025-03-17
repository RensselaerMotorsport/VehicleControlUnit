VCU_INC = ../Inc
# TODO Figure out why this is Inc but VCU_INC is be ../Inc
STM32_FAKE_INC = Inc/fake_include
TEST_INC = Test

INCS = -I$(VCU_INC) -I$(STM32_FAKE_INC)
LIBS = -lm

CC = gcc
CPPFLAGS = -DTEST_MODE
CFLAGS = -g -O0 -pthread -MMD $(INCS) $(CPPFLAGS)
LDFLAGS = $(LIBS)

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
VCU_OBJS = $(VCU_SRCS:$(VCU_DIR)/%.c=$(VCU_OBJ_DIR)/%.o)

TEST_DIR = .
TEST_SRCS = $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJ_DIR = ../Obj/Test
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
TEST_INCS = -I$(TEST_INC)
TEST_CFLAGS = $(TEST_INCS)

TEST_EXECUTABLE = test.out

all: $(TEST_EXECUTABLE)

clean:
	rm -rf $(VCU_OBJ_DIR) $(TEST_OBJ_DIR) $(TEST_EXECUTABLE)

debug:
	@echo "VCU_SRCS = $(VCU_SRCS)"
	@echo "VCU_OBJS = $(VCU_OBJS)"
	@echo "TEST_SRCS = $(TEST_SRCS)"
	@echo "TEST_OBJS = $(TEST_OBJS)"

$(TEST_EXECUTABLE): $(VCU_OBJS) $(TEST_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(VCU_OBJS) $(TEST_OBJS)

$(VCU_OBJ_DIR)/%.o: $(VCU_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) $(TEST_CFLAGS) -c $<

.PHONY: all clean debug
