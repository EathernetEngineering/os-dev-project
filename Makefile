C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS := $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

OBJ := ${C_SOURCES:.c=.o cpu/interrupt.o}

CC := /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB := /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS := -g -m32 -fno-exceptions -I${CURDIR} -Wall -Wextra -Werror

bin/os-image.bin: boot/boot.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: bin/os-image.bin
	qemu-system-i386 -drive file=bin/os-image.bin,if=floppy,format=raw

debug: bin/os-image.bin kernel.elf
	qemu-system-i386 -s -S -drive file=bin/os-image.bin,if=floppy,format=raw &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

run-vnc: bin/os-image.bin
	qemu-system-i386 -vnc :0 -drive file=bin/os-image.bin,if=floppy,format=raw

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -felf -o $@

%.bin: %.asm
	nasm $< -fbin -o $@

.PHONY: clean
clean:
	rm -f bin/* *.o *.elf *.bin cpu/*.o cpu/*.bin
	rm -f kernel/*.o kernel/*.bin drivers/*.o boot/*.bin boot/*.o

