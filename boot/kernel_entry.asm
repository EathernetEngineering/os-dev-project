[bits 64]
[extern kentry]
call kentry

mov dx, 0x0604
mov ax, 0x2000
out dx, ax

