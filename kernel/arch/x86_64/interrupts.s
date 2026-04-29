; --- In your assembly file ---

%macro isr_no_err 1
global interrupt_stub_%1
interrupt_stub_%1:
    push qword 0      ; Dummy error code
    push qword %1     ; Vector number
    jmp common_stub
%endmacro

%macro isr_with_err 1
global interrupt_stub_%1
interrupt_stub_%1:
    ; CPU already pushed error code here
    push qword %1     ; Vector number
    jmp common_stub
%endmacro

; Define stubs 0-31 (CPU Exceptions)
isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_with_err 8    ; Double Fault
isr_no_err 9
isr_with_err 10   ; Invalid TSS
isr_with_err 11   ; Segment Not Present
isr_with_err 12   ; Stack Segment Fault
isr_with_err 13   ; GPF
isr_with_err 14   ; Page Fault
isr_no_err 15
isr_no_err 16
isr_with_err 17   ; Alignment Check
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_with_err 21   ; Control Protection
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_with_err 30   ; VMM Communication
isr_no_err 31

%assign i 32
%rep 224
    isr_no_err i
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

section .data
	global interrupt_handler_table
	interrupt_handler_table:
	%assign i 0
	%rep 256
		dq interrupt_stub_%+i
	%assign i i+1
	%endrep