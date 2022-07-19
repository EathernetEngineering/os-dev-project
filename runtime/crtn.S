/* x86_64 crtn.S  */

.section .init,"ax","progbits"
	popq %rbp
	ret

.section .fini,"ax","progbits"
	popq %rbp
	ret

