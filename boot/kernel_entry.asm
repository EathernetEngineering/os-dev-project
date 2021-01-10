[bits 32]
[extern _start]
call _start
mov dx, 0x0604
mov ax, 0x2000
out dx, ax

