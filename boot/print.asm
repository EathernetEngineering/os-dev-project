
print:
	push bp
	mov bp, sp
	mov ah, 0x0E ; tty.

startloop:
	mov al, [bx]
	cmp al, 0
	je endloop

	mov ah, 0x0E
	int 0x10
	add bx, 1
	jmp startloop

endloop:
	pop bp
	ret

print_nl:
	push bp
	mov bp, sp
	push ax
	
	mov ah, 0x0E
	
	mov al, 0x0D
	int 0x10
	
	mov al, 0x0A
	int 0x10
	
	pop ax
	pop bp
	ret

