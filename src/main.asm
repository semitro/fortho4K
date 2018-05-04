global _start
global next

%include "io_lib.inc"
%include "fmachine.inc"

section .rodata
no_such_word_str:
db "No such word", 10, 0

section .text
next:
	mov w, [pc]  ; word that is going to be executed
	add pc, 8  ; pc  -> next word
;	mov w, [w] ; w    = &xt_impl 
	jmp [w]    ; rip -> xt_impl

_start:
	jmp i_init
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
	jmp .find_word_in_the_dict
.num_recieved:
	mov rdi, rax
	call print_int
	jmp .read_word
.find_word_in_the_dict:
	mov rdi, input_buffer
	call fetch_word_exec_addr
	test rax, rax
	jz .no_such_word
	jmp rax
	jmp .read_word
.no_such_word:
	mov rdi, no_such_word_str
	call print_string
	jmp .read_word
