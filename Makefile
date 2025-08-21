# 项目名与目录
PROJECT := tetromino_solver
CC      := gcc
SRC_DIR := src
INC_DIR := include
BUILD   := build
BIN     := bin

# 编译选项
CSTD    := -std=c11
WARN    := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wswitch-enum
CDEBUG  := -O0 -g3
CREL    := -O2
CPPFLAGS:= -I$(INC_DIR)
LDFLAGS :=

# 选择构建模式：debug / release（默认 debug）
MODE ?= debug
ifeq ($(MODE),release)
  CFLAGS := $(CSTD) $(WARN) $(CREL)
else ifeq ($(MODE),debug)
  CFLAGS := $(CSTD) $(WARN) $(CDEBUG)
else
  $(error Unknown MODE '$(MODE)'; use MODE=debug or MODE=release)
endif

# 可选 Sanitizer：address / undefined / thread（留空则不启用）
SAN ?=
ifeq ($(SAN),address)
  CFLAGS  += -fsanitize=address
  LDFLAGS += -fsanitize=address
else ifeq ($(SAN),undefined)
  CFLAGS  += -fsanitize=undefined
  LDFLAGS += -fsanitize=undefined
else ifeq ($(SAN),thread)
  CFLAGS  += -fsanitize=thread
  LDFLAGS += -fsanitize=thread
endif

# 源码与目标文件自动发现
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all debug release clean run test dirs

all: dirs $(BIN)/$(PROJECT)

release:
	$(MAKE) MODE=release all

debug:
	$(MAKE) MODE=debug all

dirs:
	@mkdir -p $(BUILD) $(BIN)

$(BIN)/$(PROJECT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: all
	@echo "Running with sample input tests/cases/solvable_4x1_I.in"
	@./$(BIN)/$(PROJECT) < tests/cases/solvable_4x1_I.in

test: all
	@tests/run.sh ./$(BIN)/$(PROJECT)

clean:
	$(RM) -r $(BUILD) $(BIN)