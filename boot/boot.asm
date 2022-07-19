[org 0x7C00]
STAGE_2_OFFSET equ 0x10000
STAGE_2_SIZE equ 0x500
STAGE_2_END equ (STAGE_2_OFFSET + STAGE_2_SIZE)

; Guarantee cs is set to 0x0000
jmp 0x0000:start

; Intalize other segment registers and stack
start:
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov bp, 0x7BFF
mov sp, bp

; Save drive booted from to stack
mov dh, 0x00
push dx

; Read drive parameters
mov ah, 0x08
mov dx, [bp-2] ; read drive number from stack without removing from stack
xor di, di

int 0x13

jc diskReadFailed
cmp ah, 0x00
jne diskReadFailed
and cx, 0x3F
push cx
mov ax, (STAGE_2_OFFSET >> 4)
mov es, ax
mov al, cl

; Read sectors
mov ah, 0x02
push ax
xor cx, cx
mov cl, 0x02
mov dx, [bp-2]
xor bx, bx

int 0x13

jc diskReadFailed
cmp ah, 0x00
jne diskReadFailed
pop bx
cmp al, bl
jne sectorError

; Read more sectors
pop ax
mov ah, 0x02
push ax
mov cl, 0x01
mov ch, 0x00
mov dx, [bp-2]
mov dh, 0x01
mov bx, es
add bx, (0x7C00 >> 4) ; 0x7C00 = 62 (0x3E) sectors.
mov es, bx
xor bx, bx

int 0x13

jc diskReadFailed
cmp ah, 0x00
jne diskReadFailed
pop bx
cmp al, bl
jne sectorError

mov ax, 0x0000
mov es, ax

; Jump to bootloader second stage
jmp (STAGE_2_OFFSET >> 4):0x0000

; Print error relating to sector count errors
sectorError:
mov ah, 0x0E
add al, 97 ; 97 = index of 'a', use this to easily determine error code
int 0x10
mov ah, 0x0E
mov al, 'S'
int 0x10
jmp $
; Print error if int 13h failed
diskReadFailed:
mov al, ah
add al, 97 ; 97 = index of 'a', use this to easily determine error code
mov ah, 0x0E
int 0x10
mov al, 'F'
mov ah, 0x0E
int 0x10
jmp $


; Padding from end of boot code code to MBR
times 0x1B8-($-$$) db 0x00
MBR:
	dd 0x00 ; Disk id
	dw 0x00 ; Reserved

; Partition table entry format:
;   +----------+----------------+---------------------------------------+
;   |  Offset  |  Size (bytes)  |              Description              |
;   +----------+----------------+---------------------------------------+
;   | 0x00     | 1              | Drive attributes (bit 7 = bootable)   |
;   | 0x01     | 3              | CHS of partition start                |
;   | 0x04     | 1              | Partition type                        |
;   | 0x05     | 3              | CHS address of last partition sector  |
;   | 0x08     | 4              | LBA partition start                   |
;   | 0x0C     | 4              | Number of sectors in partition        |
;   +----------+----------------+---------------------------------------+
partitionTable:
.entry1:
	db 0x80
	db 0x00, 0x01, 0x00
	db 0x7F
	db 0x01, 0x3F, 0x00
	dd 0x00
	dd 0x400

.entry2:
	db 0x00
	db 0x00
	dw 0x00
	db 0x00
	db 0x00
	dw 0x00
	dd 0x00
	dd 0x00

.entry3:
	db 0x00
	db 0x00
	dw 0x00
	db 0x00
	db 0x00
	dw 0x00
	dd 0x00
	dd 0x00

.entry4:
	db 0x00
	db 0x00
	dw 0x00
	db 0x00
	db 0x00
	dw 0x00
	dd 0x00
	dd 0x00

magicNumber: dw 0xAA55

