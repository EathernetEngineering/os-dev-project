[bits 32]

VIDEO_MEMORY equ 0xB8000
WHITE_ON_BLACK equ 0x0F

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK

	cmp al, 0
	je print_string_pm_done

	mov [edx], ax
	add ebx, 1
	add edx, 2

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret

clear_screen_pm:
	pusha
	mov edx, VIDEO_MEMORY

	mov cx, 0x0000
	mov bx, 0x0F20

clear_screen_pm_loop:
	cmp cx, 2000
	jg clear_screen_pm_done

	mov [edx], bx
	add edx, 2
	add cx, 1

	jmp clear_screen_pm_loop

clear_screen_pm_done:
	popa
	ret

