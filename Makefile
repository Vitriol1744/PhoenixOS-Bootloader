SRC_DIR := ./src
BUILD_DIR := ./build

AS := nasm

SRCS := $(shell find $(SRC_DIR) -name '*.asm')

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -i,$(INC_DIRS))

AS_FLAGS := $(INC_FLAGS)

all:
	mkdir -p $(BUILD_DIR)
	$(AS) $(AS_FLAGS) $(SRC_DIR)/boot/stage1.asm -o $(BUILD_DIR)/stage1.o

run:
	qemu-system-x86_64 $(BUILD_DIR)/stage1.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)