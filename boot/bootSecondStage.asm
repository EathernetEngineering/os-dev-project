[org 0x10000]

; Initalize stack and segments
mov ax, 0x1000
mov ds, ax
mov es, ax
xor ax, ax
mov ss, ax
xor ax, ax
mov bp, 0xFFFF
mov sp, bp

mov bx, MSG_SECOND_STAGE
call printStringR

; Get memory map.
; store it at 0x500
GetMemoryMap:
	xor bx, bx
	mov es, bx
	mov di, 0x500
	call get_memory_maps
	jc .error

	mov ax, 0x1000
	mov es, ax

	mov bx, MSG_MEMORY_MAP_LOADED
	call printStringR

	jmp enableA20

.error:
	clc

	mov ax, 0x1000
	mov es, ax

	mov bx, MSG_MEMORY_MAP_ERROR
	call printStringR

enableA20:
; Check A20 first
	pushf
	push es
	push ds

	cli

	xor ax, ax
	mov es, ax

	not ax
	mov ds, ax

	mov di, 0x0500
	mov si, 0x0510

	mov al, BYTE [es:di]
	push ax

	mov al, BYTE [ds:si]
	push ax

	mov BYTE [es:di], 0x00
	mov BYTE [ds:si], 0xFF

	cmp BYTE [es:di], 0xFF

	je .a20Enabled

; Enable A20 line.

	in al, 0x92
	or al, 2
	out 0x92, al

	mov BYTE [es:di], 0x00
	mov BYTE [ds:si], 0xFF

	cmp BYTE [es:di], 0xFF

	pop ax
	mov BYTE [ds:si], al

	pop ax
	mov BYTE [es:di], al

	je .a20Enabled

	mov bx, MSG_COULD_NOT_ENABLE_A20
	call printStringR

.a20Enabled:
	pop ds
	pop es
	popf

notifyBiosMode:
	mov ax, 0xEC00
	mov bl, 2
	int 0x15

enterProtectedMode:
	cli
	lgdt [ds:gdt32.descriptor]
	mov eax, cr0
	or al, 0x01
	mov cr0, eax
	
	; location of label `protectedModeEnabled` cannot be adressed in 16 bit space
	; so the segment with base 0x10000 is used to get into 32 bit space before setting
	; the code gdt entry back to base 0x0000 to allow storage in some memory in the
	; lower address space
	jmp CODE_FROM_16_WORKAROUND:setCodeSegBaseZero
[bits 32]
setCodeSegBaseZero:
	jmp CODE_SEG_32:protectedModeEnabled

protectedModeEnabled:
	mov ax, DATA_SEG_32
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x7FFFF
	mov esp, ebp

DetectCpuid:
	pushfd
	pop eax

	mov ecx, eax
	xor eax, 1 << 21

	push eax
	popfd

	pushfd
	pop eax

	cmp eax, ecx
	jz .noCpuid
	jmp .cpuid

.noCpuid:
	jmp $

.cpuid:
detectLongMode:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .noLongMode

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .noLongMode

	jmp .longMode

.noLongMode:
	jmp $

.longMode:
setupPaging:
	mov eax, cr0
	and eax, ~(1 << 31)
	mov cr0, eax

	mov edi, 0x1000
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3

	mov DWORD [edi], 0x2003
	add edi, 0x1000
	mov DWORD [edi], 0x3003
	add edi, 0x1000
	mov DWORD [edi], 0x4003
	add edi, 0x1000

	mov ebx, 0x03
	mov ecx, 512

.setEntry:
	mov DWORD [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .setEntry

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

enterLongMode:
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	lgdt [gdt64.descriptor]
	jmp CODE_SEG_64:longModeEnabled

[bits 64]
longModeEnabled:
	mov rax, DATA_SEG_64
	mov ds, rax
	mov ss, rax
	mov es, rax
	mov fs, rax
	mov gs, rax

	mov rbp, 0x90000
	mov rsp, rbp

	mov dx, 0x03D4 ; Port 0x03D4 VGA cursor index
	mov al, 0x0F
	out dx, al

	add dl, 0x01
	mov al, (0x3C0 & 0xFF)
	out dx, al

	sub dl, 0x01
	mov al, 0x0E
	out dx, al

	add dl, 0x01
	mov al, ((0x3C0 & 0xFF00) >> 8)
	out dx, al


; Load elf of kernel.
; File located at 0x10500, load program into memory in the way
; the liker specified
; First we need to check the magic number at the
; start of the file. Then we serch for copy from offset for memSiz bytes
; into specified location. Finally we call address sepcifed in link script
loadKernel:
	; Check magic number
	mov eax, DWORD [0x11000]
	mov ebx, DWORD [ELF_MAGIC_NUMBER]
	cmp eax, ebx
	jne .failedLoad

	; Get location of program header (0x10500 file + 0x20 index of e_pfoff)
	mov rcx, [0x11020]
	; make pointer from offset, 0x10500 location of file
	add rcx, 0x11000
	; Get size of program (rcx program header + 0x28 index of p_memsz)
	mov rbx, [rcx+0x28]
	; Get offset of start of program (rcx program header + 0x08 index of p_offset)
	mov rdx, [rcx+0x08]
	; make pointer from offset, 0x10500 location of file
	add rdx, 0x11000

	; Clear index register
	xor rdi, rdi

.loadLoop:
	; Move from source , to general register, then to destination, 4 bytes at a time
	mov rax, QWORD [rdx+rdi]
	mov QWORD [0x100000+rdi], rax
	add rdi, 8

	; Compare index against size, jump back to start of loop if not done
	cmp rdi, rbx
	jl .loadLoop

.loaded:
	; Load address of entry point then call it (0x10500 file + 0x18 e_entry)
	mov rax, [0x11018]
	call rax

	jmp $

.failedLoad:
jmp $

%include "boot/memoryMap.asm"
%include "boot/print.asm"
%include "boot/gdt.asm"

MSG_SECOND_STAGE db "Entered second stage of bootloader.", 0x0D, 0x0A, 0x00
MSG_MEMORY_MAP_FAILED db "Failed to retrive memory map.", 0x0D, 0x0A, 0x00
MSG_MEMORY_MAP_ERROR db "Int 0x15 returned with error.", 0x0D, 0x0A, 0x00
MSG_MEMORY_MAP_LOADED db "Memory map loaded to 0x00000500.", 0x0D, 0x0A, 0x00
MSG_COULD_NOT_ENABLE_A20 db "Failed to enable A20 line.", 0x0D, 0x0A, 0x00

ELF_MAGIC_NUMBER db 0x7F, 'E', 'L', 'F'

; Padding with NOP from end of code to EOF to make file 0x1000 (4096) bytes long
times 4096-($-$$) db 0x90

