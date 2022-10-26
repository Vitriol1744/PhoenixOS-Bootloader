STAGE1_BIN := stage1
IMAGE_FILE := image.hdd

BUILD_DIR := ./build
SRC_DIR := ./src

CC ?= clang
AS ?= nasm
LD ?= ld

TARGET_ARCH ?= i386-none

INC_DIRS := $(SRC_DIR) $(SRC_DIR)/stage2
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := -target $(TARGET_ARCH) $(INC_FLAGS) -m32 -ffreestanding -nostdlib -fno-pic -fno-stack-protector -fshort-wchar -mno-red-zone -masm=intel -mno-sse -mno-sse2 -mno-mmx -mno-80387
ASFLAGS := $(INC_FLAGS) -felf32
LDFLAGS := -nostdlib -no-pie -melf_i386 -s -T linker.ld

SRCS := $(shell find $(SRC_DIR)/stage2 -name '*.c' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/bootloader.bin: $(BUILD_DIR)/stage2.bin
	nasm -i$(BUILD_DIR) -i$(SRC_DIR)/stage2 $(SRC_DIR)/stage1.asm -o $@

$(BUILD_DIR)/stage2.bin: $(OBJS) linker.ld
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

rebuild:
	$(MAKE) clean
	$(MAKE)

qemu:
	qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/$(IMAGE_FILE)

#TODO: it could be good idea to generate bochs_config using makefile
bochs: bochs_config
	bochs -f bochs_config

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)