[bits 16]

; Get memory map using multiple interrupts
; CF set on error, clear on sucess
; On fail CX contains function that failed
; Stores map at es:di
; Structure of map is as follows:
;   +----------+----------------+-------------------------------------------+
;   |  Offset  |  Size (bytes)  |                Description                |
;   +----------+----------------+-------------------------------------------+
;   | 0x00     | 2              | 1K blocks of memory above 1M              |
;   | 0x02     | 2              | 1K blocks of memory between 1M and 15M    |
;   | 0x04     | 2              | 64K blocks of memory above 16M            |
;   | 0x06     | 4              | amount entries in e820 table              |
;   | 0x0A     | Stated above   | base of e820 table                        |
;   +----------+----------------+-------------------------------------------+
; See get_map_e820 for structure of e820 table
; Trashes all registers
get_memory_maps:
	push bp
	mov bp, sp
	push di
	clc
	call get_map_88
	jc .failed_88
	mov di, [ss:bp-2]
	mov [es:di], ax
	call get_map_e801
	jc .failed_e801
	mov di, [ss:bp-2]
	add di, 2
	mov [es:di], cx
	add di, 2
	mov [es:di], dx
	add di, 2
	call get_map_e820
	jc .failed_e820
	jmp .finished
.failed_88:
	mov cx, 0x88
	jmp .finished
.failed_e801:
	mov cx, 0xE801
	jmp .finished
.failed_e820:
	mov cx, 0xE820
.finished:
	pop di
	pop bp
	ret

; Get memory map using bios interrupt 15h ah=88
; CF set on error, clear on sucess
; ax = contiguous 1K blobks of memory starting at address 1M (0x100000)
get_map_88:
	clc
	mov ah, 0x88
	int 0x15
	jc .return
	test ax, ax
	jnz .return
	stc
.return:
	ret

; Get memory map using bios interrupt 15h ax=e801
; CF set on error, clear on sucess
; cx = ax = 1K blobks of memory starting at address 1M (0x100000)
; up to 15M (0xF00000)
; dx = bx = 64K blocks of memory starting at 16M (0x1000000)
get_map_e801:
	clc
	mov ax, 0xE801
	int 0x15
	jc .return ; Carry flag is already set so doing this skips redundant `stc`
	cmp ah, 0x80
	je .error
	clc
	jcxz .setAx
	jmp .return
.setAx:
	mov ax, cx
	mov bx, dx
.return:
	ret
.error:
	jmp $
	stc
	ret

; Get memory map using bios interrupt 15h ax=e820
; CF set on error, clear on sucess
; Stores map at es:di
; es:di will be a 32 bit integer conatining the number on entries,
; and es:di + 4 will be the base address of an array containing the
; folowing data structure
;   +----------+----------------+---------------------------+
;   |  Offset  |  Size (bytes)  |        Description        |
;   +----------+----------------+---------------------------+
;   | 0x00     | 4              | Low 32 bits of base       |
;   | 0x04     | 4              | High 32 bits of base      |
;   | 0x08     | 4              | Low 32 bits of length     |
;   | 0x0C     | 4              | High 32 bits of length    |
;   | 0x10     | 4              | Extended attributes       |
;   +----------+----------------+---------------------------+
get_map_e820:
	push bp
	mov bp, sp
	xor ebx, ebx
	xor si, si
	push di
.zeroTable:
	mov cx, 1540 ; 1540 = max size of returned value
	xor eax, eax
	rep stosd
	mov di, [bp-2]
.statIntLoop
	add di, 4
	mov edx, 0x0534D4150
	mov eax, 0xE820
	mov [es:di +20], DWORD 1
	mov ecx, 24
	int 0x15
	jc .failed
	mov edx, 0x0534D4150
	cmp edx, eax
	jne .failed
	test ebx, ebx
	jz .failed
	jmp .jmpin
.loop:
	mov eax, 0xE820
	mov [es:di + 20], DWORD 1
	mov ecx, 24
	int 0x15
	jc .finished
	mov edx, 0x0534D4150
.jmpin:
	jcxz .skipEntry
	cmp cl, 20
	jbe .noTest
	cmp [es:di + 20], DWORD 1
	jz .skipEntry
.noTest:
	mov ecx, [es:di + 8]
	or ecx, [es:di + 12]
	jz .skipEntry
	inc si
	add di, 24
.skipEntry:
	cmp si, 0x40
	jge .failed
	test ebx, ebx
	jne .loop
.finished:
	pop di
	mov [es:di], si
	clc
	pop bp
	ret
.failed:
	stc
	pop di
	pop bp
	ret

