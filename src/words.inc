%ifndef WORDS_INC
%define WORDS_INC
%include "macro.inc"
%include "io_lib.inc"
%include "fmachine.inc"

global root_word

extern fetch_word_hdr_addr
extern fetch_word_exec_addr
;;;;;;;;;;;;;;;;;;;;;;;;;;;
native "docol", docol
	rpush pc
	add w, 8
	mov pc, w
	jmp next

;;;;;;;;;;;;;;;;;;;;;;;;;;;
native "init", init
	lea pc, [interp]
	mov rstack, rstack_buffer
	jmp next	
;;;;;;;;;;;;;;;;;;;;;;;;;;;
colon "interpret", interpret
interp:	
	dq xt__read_word
	dq xt__read_word
	dq xt__read_word
	dq xt_show_stack
	dq xt__read_word
;;;;;;;;;;;;;;;;;;;;;;;;;;;
; read a word from stdin and store in to *input_buffer
native '_read_word', _read_word
	mov rdi, input_buffer
	mov rsi, INPUT_BUFFER_SIZE
	call read_word
	jmp next
; ( char* str - n, n.lenght )
; ( if it's not a number, n.lenght=0, n itself is undef )	
native 'parse_int', parse_int
	pop rdi
	call parse_int
	push rax ; n
	push rdx ; n.lenght
	jmp next
; ( char* buff - void* word_hdr)
native 'find_word_hdr', find_word_hdr
	pop rdi
	call fetch_word_hdr_addr
	push rax
; ( char* buff - void* word_exec_code )
native 'find_word_xt', find_word_xt
	pop rdi
	call fetch_word_exec_addr
	push rax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
