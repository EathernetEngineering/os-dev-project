C_SOURCES := $(wildcard kernel/*.c cpu/*.c klibc/*.c drivers/*.c)
C_HEADERS := $(wildcard kernel/*.h cpu/*.h klibc/*.h drivers/*.h)
CXX_SOURCES := $(wildcard kernel/*.cpp cpu/*.cpp klibc/*.cpp drivers/*.cpp)
CXX_HEADERS := $(wildcard kernel/*.hpp cpu/*.hpp klibc/*.hpp drivers/*.hpp)

COBJECTS := $(patsubst %.c,obj/%.o,$(notdir $(C_SOURCES))) obj/interrupt.o
CXXOBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(CXX_SOURCES)))
OBJECTS := $(COBJECTS) $(CXXOBJECTS)

export PROJROOTDIR := $(CURDIR)

export TARGET := x86_64

export CC := x86_64-elf-gcc
export CXX := x86_64-elf-g++
export AS := x86_64-elf-as
export AR := x86_64-elf-ar
export LD := x86_64-elf-ld

export OBJCOPY := x86_64-elf-objcopy

export GDB := x86_64-elf-gdb

CFLAGS := -g -mcmodel=large -mno-red-zone -mno-sse -mno-sse2 \
		  -mno-mmx -fexceptions -fasynchronous-unwind-tables \
		  -Isysdeps/$(TARGET) -I$(CURDIR) -include stdint.h \
		  -include stddef.h -include stdbool.h -Wall -Wextra -Werror

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-sse2 \
			-mno-mmx -fno-exceptions -fasynchronous-unwind-tables \
			-Isysdeps/$(TARGET) -I$(CURDIR) -include stdint.h -Wall -Wextra \
			-Werror -fno-rtti -fno-use-cxa-atexit

LINKFLAGS := -nostdlib

DEFINES := -D __PHYSICAL_ADDRESS_EXTENSION__=1

export LIBGCC   := $(shell $(CC) $(CXXFLAGS) --print-file-name "libgcc.a")
export CRTBEGIN := $(shell $(CC) $(CXXFLAGS) --print-file-name "crtbegin.o")
export CRTEND   := $(shell $(CC) $(CXXFLAGS) --print-file-name "crtend.o")

CRTI := obj/crti.o
CRTN := obj/crtn.o

OBJLINKLIST := $(CRTI) $(CRTBEGIN) $(OBJECTS) $(CRTEND) $(CRTN)

STDLIBRARIES := STL/bin/libstdc++.a

LIBRARIES := $(STDLIBRARIES)

.PHONY: all
all: bin/os-image.bin obj/kerneld.elf obj/kernel.elf \
	$(LIBRARIES)

bin/os-image.bin: obj/boot.bin obj/bootSecondStage.bin obj/kernel.elf
	cat $^ > $@
	dd if=/dev/zero of=$@ bs=1 count=1 seek=1548288
	chmod +x $@

obj/kerneld.elf: linkScript.ld obj/kernel_entry.o $(OBJLINKLIST)
	$(LD) -o $@ -N -T linkScript.ld $(LINKFLAGS) $(OBJLINKLIST) $(LIBGCC)

obj/kernel.elf: obj/kerneld.elf
	$(OBJCOPY) -S $^ $@

obj/crti.o: runtime/crti.S
	$(AS) -o $@ $^

obj/crtn.o: runtime/crtn.S
	$(AS) -o $@ $^

bin/OSVHD.img: obj/kernel.elf
	sudo dd if=/dev/zero of=$@ bs=1M count=1024
	sudo sfdisk $@ < OSVHD.sfdisk
	yes | sudo mkfs.ext4 $@
	-sudo mkdir /mnt/osvhd
	sudo mount -t auto -o loop $@ /mnt/osvhd
	sudo cp obj/kernel.elf /mnt/osvhd/
	sudo umount /mnt/osvhd
	-sudo rm -rf /mnt/osvhd

STL/bin/libstdc++.a:
	$(MAKE) -C STL/

.PHONY: run
run: bin/os-image.bin
	qemu-system-x86_64 \
		-chardev vc,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw

.PHONY: debug
debug: bin/os-image.bin obj/kerneld.elf
	qemu-system-x86_64 -s -S \
		-chardev vc,id=serial0,logfile=serial0.log,signal=off \
		-no-reboot \
		-D qemu.log -d int,cpu_reset,guest_errors \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw & \
	$(GDB) -ex "target remote localhost:1234" \
		-ex "symbol-file obj/kerneld.elf" \
		-ex "b _start"

.PHONY: run-vnc
run-vnc: bin/os-image.bin
	qemu-system-x86_64 -vnc :0 \
		-chardev stdio,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw

.PHONY: debug-vnc
debug-vnc: bin/os-image.bin obj/kerneld.elf
	qemu-system-x86_64 -s -vnc :0 \
		-chardev stdio,id=serial0,logfile=serial0.log,signal=off \
		-serial chardev:serial0 \
		-m size=10G \
		-drive file=bin/os-image.bin,if=ide,format=raw & \
	$(GDB) -ex "target remote localhost:1234" \
		-ex "symbol-file obj/kerneld.elf"

obj/:
	mkdir $@

bin/:
	mkdir $@

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

.PHONY: clean cleanall cleanstl cleanlibc
clean:
	rm -f bin/* obj/*

cleanall: clean cleanstl cleanlibc

cleanstl:
	$(MAKE) -C STL/ clean

cleanlibc:
	#$(MAKE) -C libc/ clean

