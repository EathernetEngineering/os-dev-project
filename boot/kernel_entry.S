/* x86_64 kernel_entry.S */

.extern kentry
.extern _init
.extern _fini

.section .text,"ax","progbits"
.global _start
.type _start,@function
.size _start,8
_start:
	call kentry
	call _init
	call kmain

	/* We should panic and die a misrible death if we reach here */
	nop

.global _exit
.type _exit,@function
.size _exit,8
_exit:
	call _fini

	/* Send shutdown signal */
	movw $0x0604, %dx
	movw $0x2000, %ax
	out %ax, %dx

