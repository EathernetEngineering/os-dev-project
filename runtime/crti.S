/* x86_64 crti.S  */

.section .init,"ax","progbits"
.global _init
.type _init,@function
.size _start,8
_init:
	pushq %rbp
	movq %rsp, %rbp

.section .fini,"ax","progbits"
.global _fini
.type _fini,@function
.size _fini,8
_fini:
	pushq %rbp
	movq %rsp, %rbp

