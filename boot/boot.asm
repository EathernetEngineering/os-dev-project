[org 0x7C00] ; Offset everything by 0x7C00.
KERNEL_OFFSET equ 0x1000 ; same as defined in linker

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print
call print_nl

call load_kernel
call switch_to_pm

hlt

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/boot_sector_disk.asm"
%include "boot/32_bit_gdt.asm"
%include "boot/32_bit_print.asm"
%include "boot/32_bit_switch.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_nl

	mov bx, KERNEL_OFFSET
	mov dh, 16 ; The kenral will be much larger in the future, loaf 8KbB into memory for now
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call clear_screen_pm
	call print_string_pm
	call KERNEL_OFFSET
	mov dx, 0x0604
	mov ax, 0x2000
	out dx, ax

BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0
MSG_LOAD_KERNEL db "Loading kernal into memory...", 0

; Fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
; Magic number
dw 0xAA55

