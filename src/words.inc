%ifndef WORDS_INC
%define WORDS_INC
%include "macro.inc"
%include "io_lib.inc"

extern fetch_word_hdr_addr
global root_word

native '.S', show_stack
	mov rdi, 42
	call print_char
;	jmp next

native '.', pop_stack
	mov rdi, 43
	call print_char
;	jmp next


root_word: dq wh_pop_stack ; it's a variable void* 
			   ; pointing to the head of the list
%endif ; WORDS_INC
