[bits 16]
printStringR:
	push bp
	mov bp, sp

	; push modified registers to stack
	push ax
	push bx

	mov ah, 0x0E ; set ah to 0x0E (int 0x10 tty write)
	xor si, si ; zero si (index)

.startLoop:
	mov al, BYTE [bx]
	cmp al, 0x00
	je printStringR.endLoop

	add bx, 1
	mov ah, 0x0E
	int 0x10
	jmp printStringR.startLoop

.endLoop:

	; pop register values from stack
	pop bx
	pop ax

	pop bp
	ret

[bits 32]
printStringP:
	push bp
	mov bp, sp

	push ax
	push esi
	push edx

	mov ah, 0x0F
	xor esi, esi
	mov edx, 0x000B8000

.startLoop:
	mov al, BYTE [bx+si]
	cmp al, 0x00
	je .endLoop

	mov WORD [edx], ax
	add si, 1
	add edx, 2
	jmp .startLoop

.endLoop:
	pop edx
	pop esi
	pop ax

	pop bp
	ret

clearScreenP:
	push ebx
	push esi

	mov ebx, 0x000B8000
	mov esi, 0x0FA0

.loop:
	mov [ebx], DWORD 0x00000000
	add ebx, 4
	sub esi, 4
	jnz .loop

	pop esi
	pop ebx
	ret

[bits 64]
printStringL:
	push rbp
	mov rbp, rsp

	push rax
	push rsi
	push rdx

	mov ah, 0x0F
	xor rsi, rsi
	mov rdx, 0x000B8000

.startLoop:
	mov al, BYTE [rbx+rsi]
	cmp al, 0x00
	je .endLoop

	mov WORD [edx], ax
	add si, 1
	add rdx, 2
	jmp .startLoop

.endLoop:
	pop rdx
	pop rsi
	pop rax

	pop rbp
	ret

clearScreenL:
	sub rsp, 16
	mov [rsp], rbx
	mov [rsp+8], rsi

	mov rbx, 0x000B8000
	mov rsi, 0x0FA0

.loop:
	mov [rbx], DWORD 0x00000000
	add rbx, 4
	sub rsi, 4
	jnz .loop

	mov rsi, [rsp+8]
	mov rbx, [rsp]
	add rsp, 16
	ret

