global _start


%include "io_lib.inc"
%include "fmachine.inc"

section .text
_start:
.interp_loop:
.read_word:
	mov rdi, input_buffer 
	mov rsi, INPUT_BUFFER_SIZE
	call read_word

.isNum:
	mov rdi, input_buffer
	call parse_int
	test rdx, rdx
	jne .num_recieved
	jmp .read_word
.num_recieved:
	mov rdi, rax
	call print_int
	jmp .read_word
