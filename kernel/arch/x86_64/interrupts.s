; Define the stubs
%macro interrupt_stub 1
global interrupt_stub_%1
interrupt_stub_%1:
	push qword 0
	push qword %1
	jmp common_stub
%endmacro

; Generate all 256
%assign i 0
%rep 256
	interrupt_stub i
%assign i i+1
%endrep

extern common_interrupt_handler
common_stub:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rdi, rsp
	call common_interrupt_handler

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	add rsp, 16
	iretq

; This is what your loop will use!
section .data
global interrupt_handler_table
interrupt_handler_table:
	%assign i 0
	%rep 256
		dq interrupt_stub_%+i
	%assign i i+1
	%endrep