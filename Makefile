C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c runtime/*.c)
C_HEADERS := $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h runtime/*.h)
CXX_SOURCES := $(wildcard kernel/*.cpp drivers/*.cpp cpu/*.cpp libc/*.cpp runtime/*.cpp)
CXX_HEADERS := $(wildcard kernel/*.hpp drivers/*.hpp cpu/*.hpp libc/*.hpp runtime/*.hpp)

COBJECTS := $(patsubst %.c,obj/%.o,$(notdir $(C_SOURCES))) obj/interrupt.o
CXXOBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(CXX_SOURCES)))
OBJECTS := $(COBJECTS) $(CXXOBJECTS)

TARGET := x86_64

CC := /usr/x86_64elfgcc/bin/x86_64-elf-gcc
CXX := /usr/x86_64elfgcc/bin/x86_64-elf-g++
AS := /usr/x86_64elfgcc/bin/x86_64-elf-as
AR := /usr/x86_64elfgcc/bin/x86_64-elf-ar
LD := /usr/x86_64elfgcc/bin/x86_64-elf-ld

OBJCOPY := /usr/x86_64elfgcc/bin/x86_64-elf-objcopy

GDB := /usr/x86_64elfgcc/bin/x86_64-elf-gdb

CFLAGS := -g -mcmodel=large -mno-red-zone -mno-sse -mno-mmx -mno-sse2 -fexceptions -fasynchronous-unwind-tables -Isysdeps/$(TARGET) -I$(CURDIR) -include stdint.h -include stddef.h -include stdbool.h -Wall -Wextra -Werror

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-mmx -mno-sse2 -fexceptions -fasynchronous-unwind-tables -Isysdeps/$(TARGET) -I$(CURDIR) -include stdint.h -include stddef.h -include stdbool.h -Wall -Wextra -Werror

DEFINES := -D __PHYSICAL_ADDRESS_EXTENSION__=1

.PHONY: all
all: bin/os-image.bin obj/kerneld.elf obj/kernel.elf bin/OSVHD.img STL/bin/libstdc++.a

bin/os-image.bin: obj/boot.bin obj/bootSecondStage.bin obj/kernel.elf
	cat $^ > $@
	dd if=/dev/zero of=$@ bs=1 count=1 seek=1548288
	chmod +x $@

obj/kerneld.elf: linkScript.ld obj/kernel_entry.o $(OBJECTS)
	$(LD) -o $@ -N -T $^

obj/kernel.elf: obj/kerneld.elf
	$(OBJCOPY) -S $^ $@

bin/OSVHD.img: obj/boot.bin obj/bootSecondStage.bin obj/kernel.elf
	dd if=/dev/zero of=$@ bs=1M count=512
	sfdisk $@ < OSVHD.sfdisk
	mkdir /mnt/osvhd
	

STL/bin/libstdc++.a:
	$(MAKE) -C STL/

.PHONY: run
run: bin/os-image.bin
	qemu-system-x86_64 -chardev vc,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw

.PHONY: debug
debug: bin/os-image.bin obj/kerneld.elf
	qemu-system-x86_64 -s -S -chardev vc,id=serial0,logfile=serial0.log,signal=off \
		-no-reboot \
		-D qemu.log -d int,cpu_reset,guest_errors \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw & \
	$(GDB) -ex "target remote localhost:1234" \
		-ex "symbol-file obj/kerneld.elf" \
		-ex "b kentry"

.PHONY: run-vnc
run-vnc: bin/os-image.bin
	qemu-system-x86_64 -vnc :0 -chardev stdio,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw

.PHONY: debug-vnc
debug-vnc: bin/os-image.bin obj/kerneld.elf
	qemu-system-x86_64 -s -vnc :0 -chardev stdio,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw & \
	$(GDB) -ex "target remote localhost:1234" \
		-ex "symbol-file obj/kerneld.elf"

obj/%.o: */%.c $(C_HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) -ffreestanding -c $< -o $@

obj/%.o: */%.cpp $(CXX_HEADERS)
	$(CXX) $(CXXFLAGS) $(DEFINES) -ffreestanding -c $< -o $@

obj/%.o: */%.asm
	nasm $< -felf64 -o $@

obj/%.o: */%.S
	$(AS) --64 -g $< -o $@

obj/%.bin: */%.asm
	nasm $< -fbin -o $@

.PHONY: clean
clean:
	rm -f bin/* obj/*
	$(MAKE) -C STL/ clean

