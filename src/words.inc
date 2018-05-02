%ifndef WORDS_INC
%define WORDS_INC
%include "macro.inc"
%include "io_lib.inc"
%include "fmachine.inc"

global root_word

extern fetch_word_hdr_addr
extern fetch_word_exec_addr
;;;;;;;;;;;;;;;;;;;;;;;;;;;
; to enter a colon word
native "docol", docol
	rpush pc
	add w, 8
	mov pc, w
	jmp next

; to exit a colon word
native "leave", leave
	rpop pc
	jmp next

;;;;;;;;;;;;;;;;;;;;;;;;;;;
native "init", init
	lea pc, [interp]
	mov rstack, rstack_buffer
	jmp next	

;;;;;;;MAIN_THING;;;;;;;;;;
colon "interpret", interpret
interp:	
	dq xt_get_input_buffer 
	dq xt_read_word
	dq xt_get_input_buffer
	dq xt_parse_int
	branch_if .num ; if it's num
	dq xt_drop ; parse int returns 2 args
	dq xt_get_input_buffer
.word:
	dq xt_find_word_xt ; ( void * code )
	dq xt_dup ; if there's no such word, tos=0
	branch_if .word_exists
.no_word:
	dq xt_print_no_word	
	dq xt_drop ; clear 0 code value
	branch interp

.word_exists:
	dq xt_execute
.num:	
	; the number's already on the top of the stack
	branch interp

colon "print_no_word", print_no_word
	dq xt_get_input_buffer
	dq xt_print
	dq xt_lit
       	dq ':'
	dq xt_emit
	dq xt_get_no_such_word_buffer
	dq xt_print
	dq xt_emit
	dq xt_leave

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
; ( void* code - )
native 'execute', execute
	pop rax
	jmp rax
; (char* str - )
; prints string
native 'print', print
	pop rdi
	call print_string
	jmp next

; (char c - )
; prints single character to stdout
native 'emit', emit
	pop rdi
	call print_char
	jmp next
; prints \n character
native 'cr', cr
	call print_newline
	jmp next

native 'get_no_such_word_buffer', get_no_such_word_buffer
	push no_such_word_str
	jmp next

native 'get_input_buffer', get_input_buffer
	push input_buffer
	jmp next

; ( char* - )
; read a word from stdin and store it into the addr
native 'read_word', read_word
	pop rdi
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
; it's helpful for debug
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

; push next dq thing onto the stack
native 'lit', lit
	push qword[pc] ; push next ceil
	add pc, 8 ; next ceil's data, not a command
	jmp next

native 'dup', dup
	;push qword[rsp]
	pop rax
	push rax
	push rax
	jmp next

native 'drop', drop
	pop rax
;	add rsp, 8
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
