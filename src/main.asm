global _start

extern parse_int
extern read_word
extern read_char
extern print_string

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
