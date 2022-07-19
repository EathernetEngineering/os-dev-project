.section .text,"ax","progbits"

.extern isr_handler
.extern irq_handler

.macro SAVE_ARGS_IRQ
	cld
	movq %rdi, (0x50)
	movq %rsi, (0x48)
	movq %rdx, (0x40)
	movq %rcx, (0x38)
	movq %rax, (0x30)
	movq %r8, (0x28)
	movq %r9, (0x20)
	movq %r10, (0x18)
	movq %r11, (0x10)

	movq %rbp, (0x00)
.endm

.macro RESTORE_ARGS_IRQ
	movq (0x00), %rbp

	movq (0x50), %rdi
	movq (0x48), %rsi
	movq (0x40), %rdx
	movq (0x38), %rcx
	movq (0x30), %rax
	movq (0x28), %r8
	movq (0x20), %r9
	movq (0x18), %r10
	movq (0x10), %r11
.endm

isrCommonStub:
	SAVE_ARGS_IRQ
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	mov %rsp, %rax
	sub 32, %rax
	pushq %rax
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	xorq %rax, %rax
	movw %ds, %ax
	pushq %rax
	movq $0x10, %rax
	movw %ds, %ax
	movw %es, %ax
	movw %fs, %ax
	movw %gs, %ax
	movq %rsp, %rdi

	call isrHandler

	popq %rax
	movw %ds, %ax
	movw %es, %ax
	movw %gs, %ax
	movw %gs, %ax
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	addq $8, %rsp
	popq %rbx
	popq %rdx
	popq %rcx
	popq %rax
	RESTORE_ARGS_IRQ
	addq $16, %rsp
	iretq

irqCommonStub:
	SAVE_ARGS_IRQ
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	mov %rsp, %rax
	sub 32, %rax
	pushq %rax
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movw %ds, %ax
	pushq %rax
	movq $0x10, %rax
	movw %ds, %ax
	movw %es, %ax
	movw %fs, %ax
	movw %gs, %ax
	movq %rsp, %rdi

	call irqHandler

	popq %rbx
	movw %ds, %bx
	movw %es, %bx
	movw %fs, %bx
	movw %gs, %bx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	pushq %rdi
	pushq %rsi
	pushq %rbp
	addq $8, %rsp
	pushq %rbx
	pushq %rdx
	pushq %rcx
	pushq %rax
	RESTORE_ARGS_IRQ
	iretq

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

.global svr

# 0: Divide By Zero Exception
isr0:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $0, %rsp
	jmp isrCommonStub

# 1: Debug Exception
isr1:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $1, %rsp
	jmp isrCommonStub

# 2: Non Maskable Interrupt Exception
isr2:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $2, %rsp
	jmp isrCommonStub

# 3: Int 3 Exception
isr3:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $3, (%rsp)
	jmp isrCommonStub

# 4: INTO Exception
isr4:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $4, (%rsp)
	jmp isrCommonStub

# 5: Out of Bounds Exception
isr5:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $5, (%rsp)
	jmp isrCommonStub

# 6: Invalid Opcode Exception
isr6:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $6, (%rsp)
	jmp isrCommonStub

# 7: Coprocessor Not Available Exception
isr7:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $7, (%rsp)
	jmp isrCommonStub

# 8: Double Fault Exception (With Error Code!)
isr8:
	cli
	sub $8, %rsp
	movq $8, (%rsp)
	jmp isrCommonStub

# 9: Coprocessor Segment Overrun Exception
isr9:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $9, (%rsp)
	jmp isrCommonStub

# 10: Bad TSS Exception (With Error Code!)
isr10:
	cli
	sub $8, %rsp
	movq $10, (%rsp)
	jmp isrCommonStub

# 11: Segment Not Present Exception (With Error Code!)
isr11:
	cli
	sub $8, %rsp
	movq $11, (%rsp)
	jmp isrCommonStub

# 12: Stack Fault Exception (With Error Code!)
isr12:
	cli
	sub $8, %rsp
	movq $12, (%rsp)
	jmp isrCommonStub

# 13: General Protection Fault Exception (With Error Code!)
isr13:
	cli
	sub $8, %rsp
	movq $13, (%rsp)
	jmp isrCommonStub

# 14: Page Fault Exception (With Error Code!)
isr14:
	cli
	sub $8, %rsp
	movq $14, (%rsp)
	jmp isrCommonStub

# 15: Reserved Exception
isr15:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $15, (%rsp)
	jmp isrCommonStub

# 16: Floating Point Exception
isr16:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $16, (%rsp)
	jmp isrCommonStub

# 17: Alignment Check Exception
isr17:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $17, (%rsp)
	jmp isrCommonStub

# 18: Machine Check Exception
isr18:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $18, (%rsp)
	jmp isrCommonStub

# 19: Reserved
isr19:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $19, (%rsp)
	jmp isrCommonStub

# 20: Reserved
isr20:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $20, (%rsp)
	jmp isrCommonStub

# 21: Reserved
isr21:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $21, (%rsp)
	jmp isrCommonStub

# 22: Reserved
isr22:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $22, (%rsp)
	jmp isrCommonStub

# 23: Reserved
isr23:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $23, (%rsp)
	jmp isrCommonStub

# 24: Reserved
isr24:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $24, (%rsp)
	jmp isrCommonStub

# 25: Reserved
isr25:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $25, (%rsp)
	jmp isrCommonStub

# 26: Reserved
isr26:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $26, (%rsp)
	jmp isrCommonStub

# 27: Reserved
isr27:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $27, (%rsp)
	jmp isrCommonStub

# 28: Reserved
isr28:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $28, (%rsp)
	jmp isrCommonStub

# 29: Reserved
isr29:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $29, (%rsp)
	jmp isrCommonStub

# 30: Reserved
isr30:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $30, (%rsp)
	jmp isrCommonStub

# 31: Reserved
isr31:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $31, (%rsp)
	jmp isrCommonStub

irq0:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $32, (%rsp)
	jmp irqCommonStub

irq1:
	cli
	sub $16, %rsp
	movq $1, 8(%rsp)
	movq $33, (%rsp)
	jmp irqCommonStub

irq2:
	cli
	sub $16, %rsp
	movq $2, 8(%rsp)
	movq $34, (%rsp)
	jmp irqCommonStub

irq3:
	cli
	sub $16, %rsp
	movq $3, 8(%rsp)
	movq $35, (%rsp)
	jmp irqCommonStub

irq4:
	cli
	sub $16, %rsp
	movq $4, 8(%rsp)
	movq $36, (%rsp)
	jmp irqCommonStub

irq5:
	cli
	sub $16, %rsp
	movq $5, 8(%rsp)
	movq $37, (%rsp)
	jmp irqCommonStub

irq6:
	cli
	sub $16, %rsp
	movq $6, 8(%rsp)
	movq $38, (%rsp)
	jmp irqCommonStub

irq7:
	cli
	sub $16, %rsp
	movq $7, 8(%rsp)
	movq $39, (%rsp)
	jmp irqCommonStub

irq8:
	cli
	sub $16, %rsp
	movq $8, 8(%rsp)
	movq $40, (%rsp)
	jmp irqCommonStub

irq9:
	cli
	sub $16, %rsp
	movq $9, 8(%rsp)
	movq $41, (%rsp)
	jmp irqCommonStub

irq10:
	cli
	sub $16, %rsp
	movq $10, 8(%rsp)
	movq $42, (%rsp)
	jmp irqCommonStub

irq11:
	cli
	sub $16, %rsp
	movq $11, 8(%rsp)
	movq $43, (%rsp)
	jmp irqCommonStub

irq12:
	cli
	sub $16, %rsp
	movq $12, 8(%rsp)
	movq $44, (%rsp)
	jmp irqCommonStub

irq13:
	cli
	sub $16, %rsp
	movq $13, 8(%rsp)
	movq $45, (%rsp)
	jmp irqCommonStub

irq14:
	cli
	sub $16, %rsp
	movq $14, 8(%rsp)
	movq $46, (%rsp)
	jmp irqCommonStub

irq15:
	cli
	sub $16, %rsp
	movq $15, 8(%rsp)
	movq $47, (%rsp)
	jmp irqCommonStub

svr:
	cli
	sub $16, %rsp
	movq $0, 8(%rsp)
	movq $255, (%rsp)
	jmp irqCommonStub

