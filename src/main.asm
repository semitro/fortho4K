global _start

extern parse_int
extern read_word

%include "io_lib.inc"
%include "fmachine.inc"

section .text
_start:
.interp_loop:
.read_word:
	mov rdi, input_buffer 
	call read_word
.isNum:
	mov rdi, input_buffer	
