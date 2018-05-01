global _start
global next

%include "io_lib.inc"
%include "fmachine.inc"

section .text
_start:
.interp_loop:
next:
.read_word:
	mov rdi, input_buffer 
	mov rsi, INPUT_BUFFER_SIZE
	call read_word

.isNum:
	mov rdi, input_buffer
	call parse_int
	test rdx, rdx
	jne .num_recieved
	jmp .find_word_in_the_dict
.num_recieved:
	mov rdi, rax
	call print_int
	jmp .read_word
.find_word_in_the_dict:
	mov rdi, input_buffer
	call fetch_word_hdr_addr
	mov  rdi, rax
	call print_int
	jmp .read_word
