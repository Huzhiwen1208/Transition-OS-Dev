AsmCompile=nasm
CCompile=gcc
IMG=img/transition.img

GccFlags=-m32 -fno-builtin -fno-stack-protector
GccFlags+=-w -nostdinc -nostdlib -fno-pic -fno-pie -g
LdFlags=-m elf_i386 -static

TARGET=src/target
BootLoader=src/bootloader
KernelPath=src/kernel
ELFKernel=$(TARGET)/kernel/os.elf
NakedKernel=$(TARGET)/kernel/os.bin
KernelSourceFile=$(wildcard $(KernelPath)/*.c) $(wildcard $(KernelPath)/*.asm)
KernelSourceFile+=$(wildcard $(KernelPath)/*/*.c) $(wildcard $(KernelPath)/*/*.asm)

KernelOBJ=$(patsubst $(KernelPath)/%.asm, $(TARGET)/kernel/%.o, $(filter %.asm, $(KernelSourceFile)))
KernelOBJ+=$(patsubst $(KernelPath)/%.c, $(TARGET)/kernel/%.o, $(filter %.c, $(KernelSourceFile)))
ENTRYPOINT=0x7e00

run: build
	qemu-system-i386 -m 32M \
		-drive file=img/transition.img,if=ide,index=0,media=disk,format=raw

build: $(TARGET) $(IMG)

.PHONY: $(TARGET)
$(TARGET):
ifeq ($(wildcard $(TARGET)),)
	@mkdir -p $(TARGET)/bootloader
	@mkdir -p $(TARGET)/kernel
	@mkdir -p $(TARGET)/kernel/common
	@mkdir -p $(TARGET)/kernel/console
	@mkdir -p $(TARGET)/kernel/lib
	@mkdir -p $(TARGET)/kernel/memory
endif

# .c, .asm ---> .o ------
$(TARGET)/kernel/%.o: $(KernelPath)/%.c
	$(CCompile) $(GccFlags) -c -o $@ $<

$(TARGET)/kernel/%.o: $(KernelPath)/%.asm
	$(AsmCompile) -f elf32 -g -o $@ $<
# ----- .c, .asm ---> .o

# bootloader  -------
$(TARGET)/bootloader/%.bin: $(BootLoader)/%.asm
	$(AsmCompile) -o $@ $<
# ------ bootloader

# kernel made ----- 
# replace ld, objcopy in linux:binutils with ld.lld, llvm-objcopy in macos:llvm
$(ELFKernel): $(KernelOBJ)
	ld $(LdFlags) -Ttext $(ENTRYPOINT) $^ -o $@
$(NakedKernel): $(ELFKernel)
	objcopy -O binary $< $@
# ------ kernel made

# img made --------
$(IMG): $(TARGET)/bootloader/boot.bin $(TARGET)/bootloader/loader.bin \
	$(NakedKernel) image
	dd if=$(word 1, $^) of=$@ bs=512 count=1 conv=notrunc
	dd if=$(word 2, $^) of=$@ bs=512 count=3 seek=1 conv=notrunc
	dd if=$(word 3, $^) of=$@ bs=512 count=200 seek=4 conv=notrunc

image:
ifeq ($(wildcard img),)
	@mkdir img
endif
ifeq ($(wildcard $(IMG)),)
	bximage -q -hd=128 -mode=create -sectsize=512 -imgmode=flat $(IMG)
endif
# ------- img made

debug: build 
	qemu-system-i386 -m 32M \
		-drive file=img/transition.img,if=ide,index=0,media=disk,format=raw -s -S

.PHONY: clean
clean:
	rm -rf *.bin *.o *.lock *.ini *.s *.asm
	rm -rf img
	rm -rf src/target