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
	;push input_buffer
	dq xt_read_word
	dq xt_parse_int
	dq xt_42
	branch_if .num ; if it's num
	dq xt_42
	branch interp
.num:	
	dq xt_43
	dq xt_43
	branch interp
	mov rdi, 43
	call print_char
	call print_char


;;;;;;;; BRANCHES ;;;;;
; ( bool tos - )
; run next word if tos != 0
; DANGEROUS: DON'T use macro "branch" after!
; 'cause it stores addr as word after itself 

native "run_if", run_if
	pop rax
	test rax, rax
	jnz next  ; run  next word
	add pc, 8 ; skip next word
	jmp next

; ( bool value - )
; Does branch if tos != 0
native "branch_if", branch_if
	pop rax
	test rax, rax
	jnz i_branch
	add pc, 8 ; skip next addr
	jmp next

; jmp to label right after word
; only in precompile
native "branch", branch 
	mov pc, [pc] ; pc = void*. Addr is in the next ceil
	jmp next

;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ( char* - )
; read a word from stdin and store it into the addr
native 'read_word', read_word
	push 	input_buffer
	pop rdi
	mov rsi, INPUT_BUFFER_SIZE
	call read_word
	jmp next
	
; ( char* str - n, n.lenght )
; ( if it's not a number, n.lenght=0, n itself is undef )	
native 'parse_int', parse_int
	push input_buffer ; temporary
	pop rdi
	call parse_int
	push rax ; n
	push rdx ; n.lenght
	jmp next

colon 'is_num', is_num
	dq xt_parse_int
	dq xt_swap
	;dq xt_pop


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
; it's helpful for debub
native '42', 42
	mov rdi, 42
	call print_char
	call print_newline
	jmp next

native '43', 43
	mov rdi, 43
	call print_char
	call print_newline
	jmp next

native '.', .
	pop rdi
	call print_int
	call print_newline
	jmp next

; ( a b - b a ) 
native 'swap', swap
	pop rax
	pop rcx
	push rax
	push rcx
	jmp next
;;;;;;;;;;;; ARIFMETIC ;;;;;;;;;;;;;;;
root_word: dq wh_. ; it's a variable void* 
			   ; pointing to the head of the list
%endif ; WORDS_INC
