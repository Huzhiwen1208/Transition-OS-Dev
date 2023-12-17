AsmCompile=nasm
CCompile=gcc
IMG=img/transition.img
FSImage=img/fs.img

GccFlags=-m32 -fno-builtin -fno-stack-protector -march=pentium
GccFlags+=-w -nostdinc -nostdlib -fno-pic -fno-pie -g
LdFlags=-m elf_i386 -static

TARGET=target
BootLoader=src/bootloader
KernelPath=src/kernel
UserPath=src/user

ELFKernel=$(TARGET)/kernel/os.elf
NakedKernel=$(TARGET)/kernel/os.bin
KernelSourceFile=$(wildcard $(KernelPath)/*.c) $(wildcard $(KernelPath)/*.asm)
KernelSourceFile+=$(wildcard $(KernelPath)/*/*.c) $(wildcard $(KernelPath)/*/*.asm)
UserSourceFile=$(wildcard $(UserPath)/*.c)

KernelOBJ=$(patsubst $(KernelPath)/%.asm, $(TARGET)/kernel/%.o, $(filter %.asm, $(KernelSourceFile)))
KernelOBJ+=$(patsubst $(KernelPath)/%.c, $(TARGET)/kernel/%.o, $(filter %.c, $(KernelSourceFile)))
UserOBJ=$(patsubst $(UserPath)/%.c, $(TARGET)/user/%.o, $(filter %.c, $(UserSourceFile)))
ENTRYPOINT=0x7e00

run: build
	qemu-system-i386 -m 32M \
		-drive file=img/transition.img,if=ide,index=0,media=disk,format=raw \
		-drive file=img/fs.img,if=ide,index=1,media=disk,format=raw

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
	@mkdir -p $(TARGET)/kernel/ds
	@mkdir -p $(TARGET)/kernel/int
	@mkdir -p $(TARGET)/kernel/descriptor
	@mkdir -p $(TARGET)/kernel/process
	@mkdir -p $(TARGET)/kernel/syscall
	@mkdir -p $(TARGET)/user
	@mkdir -p $(TARGET)/kernel/device
	@mkdir -p $(TARGET)/kernel/disk
	@mkdir -p $(TARGET)/kernel/fs
endif

# .c, .asm ---> .o ------
$(TARGET)/kernel/%.o: $(KernelPath)/%.c
	$(CCompile) $(GccFlags) -c -o $@ $<
$(TARGET)/user/%.o: $(UserPath)/%.c
	$(CCompile) $(GccFlags) -c -o $@ $<
$(TARGET)/kernel/%.o: $(KernelPath)/%.asm
	$(AsmCompile) -f elf32 -g $< -o $@ 
# ----- .c, .asm ---> .o

# bootloader  -------
$(TARGET)/bootloader/%.bin: $(BootLoader)/%.asm
	$(AsmCompile) -o $@ $<
# ------ bootloader

# kernel made ----- 
$(ELFKernel): $(KernelOBJ) $(UserOBJ)
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
	bximage -q -hd=16 -mode=create -sectsize=512 -imgmode=flat $(IMG)
endif
ifeq ($(wildcard $(FSImage)),)
	bximage -q -hd=64 -mode=create -sectsize=512 -imgmode=flat $(FSImage)
endif
# ------- img made

debug: build 
	qemu-system-i386 -m 32M \
		-drive file=img/transition.img,if=ide,index=0,media=disk,format=raw \
		-drive file=img/fs.img,if=ide,index=1,media=disk,format=raw \
		-s -S
		

run-mac:
	@make -f Makefile.mac run

debug-mac:
	@make -f Makefile.mac debug

build-mac:
	@make -f Makefile.mac build

.PHONY: clean
clean:
	rm -rf *.bin *.o *.lock *.ini *.s *.asm
	rm -rf img
	rm -rf src/target
	rm -rf target
