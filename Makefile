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
	$(AS) $(AS_FLAGS) $(SRC_DIR)/boot/stage2.asm -o $(BUILD_DIR)/stage2.o
	cat $(BUILD_DIR)/stage1.o $(BUILD_DIR)/stage2.o > $(BUILD_DIR)/boot.bin

run:
	qemu-system-x86_64 $(BUILD_DIR)/boot.bin
debug:
	qemu-system-x86_64 -S -s $(BUILD_DIR)/boot.bin

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)