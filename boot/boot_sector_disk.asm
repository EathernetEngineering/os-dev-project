disk_load:
	pusha
	push dx

	mov ah, 0x02
	mov al, dh
	mov cl, 0x02

	mov ch, 0x00

	mov dh, 0x00

	int 0x13
	jc disk_error

	pop dx
	cmp al, dh
	jne sectors_error
	popa
	ret

disk_error:
	push bx
	mov bx, DISK_ERROR
	call print
	call print_nl
	
	mov dx, ax
	ror dx, 8
	and dx, 0x000f
	add dl, 0x30
	cmp dl, 0x39
	jle noadd1
	add dl, 7
noadd1:
	mov [HEX_OUT+3], dl
	mov dx, ax
	ror dx, 4
	and dx, 0x000F
	add dl, 0x30
	cmp dl, 0x39
	jle noadd2
	add dl, 7
noadd2:
	mov [HEX_OUT+2], dl

	mov bx, HEX_OUT
	call print

	pop bx
	jmp hang_on_disk_error

sectors_error:
	push bx
	mov bx, SECTORS_ERROR
	call print
	call print_nl
	pop bx

hang_on_disk_error:
	jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0

HEX_OUT:
db "0x00", 0

