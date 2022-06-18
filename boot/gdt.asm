[bits 32]
gdt32:
.start:
	dd 0x00000000
	dd 0x00000000

.code:
	dw 0xFFFF     ; limit 0:15
	dw 0x0000     ; base 0:15
	db 0x00       ; base 16:23
	db 0b10011010 ; access byte
	db 0b11001111 ; limit 16:19 flags
	db 0x00       ; base 24:31

.data:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0b10010010
	db 0b11001111
	db 0x00

.longJumpFrom16Workaround:
	dw 0xFFFF
	dw 0x0000
	db 0x01
	db 0b10011010
	db 0b11001111
	db 0x00

.end:
.descriptor:
	dw gdt32.end - gdt32.start - 1
	dd gdt32.start

CODE_FROM_16_WORKAROUND equ gdt32.longJumpFrom16Workaround - gdt32.start
CODE_SEG_32 equ gdt32.code - gdt32.start
DATA_SEG_32 equ gdt32.data - gdt32.start

[bits 64]
gdt64:
.start:
	dw 0x0000FFFF
	dw 0x00000000
	db 0x00
	db 0x00
	db 0x01
	db 0x00

.code:
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0b10011010
	db 0b10101111
	db 0x00

.data:
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0b10010010
	db 0b00000000
	db 0x00

.end:
.descriptor:
	dw gdt64.end - gdt64.start - 1
	dd gdt64.start

CODE_SEG_64 equ gdt64.code - gdt64.start
DATA_SEG_64 equ gdt64.data - gdt64.start

