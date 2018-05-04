%ifndef WORDS_INC
%define WORDS_INC
%include "macro.inc"
%include "io_lib.inc"
%include "fmachine.inc"

%define _IMMEDIATE 0x01

section .rdata
colon_word_string: db "colon", 0
section .data
extern fetch_word_hdr_addr
extern fetch_word_exec_addr
;;;;;;;;;;;;;;;;;;;;;;;;;;;
; to enter a colon word
native "docol", docol
	rpush pc
	call print_char
	add w, 8
	mov pc, w
	jmp next

; to exit a colon word
native "leave", leave
	rpop pc
	jmp next

native "bye", bye
	call exit

;;;;;;;;;;;;;;;;;;;;;;;;;;;
native "init", init
;	lea pc, [interp] ; load interpet
;	lea pc, [interp+8]
	lea pc, [interp]
	mov rstack, rstack_buffer
	mov [inital_rsp], rsp ; let .S works
	mov rax, [default_root]
	mov [root_word], rax
	jmp next	
;;
colon "test_colon", test_colon
	dq xt_42
	dq xt_43
	dq xt_leave

colon "test_colon_in_colon", test_colon_in_colon
	dq xt_test_colon
	dq xt_43
	dq xt_leave

;;;;;;;;;;; COMPILE ;;;;;;;;
colon  ":", switch_to_compiler
	dq xt_leave
	dq xt_print_welcome
	dq xt_get_input_buffer
	dq xt_read_word
	dq xt_switch_state_to_compiler
;	dq xt_lit
;	dq colon_word_string 
;	dq xt_create_word
	; don't forget about non-existing words in future!
	
	dq xt_get_input_buffer
	dq xt_read_word

native ":to_compiler", switch_state_to_compiler
	mov qword [state], STATE_COMPILER
	mov rdx, [root_word]
	mov [new_root], rdx
	jmp next

native ":to_interpret", switch_state_to_interpret
	mov qword [state], STATE_INTERPRET
	jmp next


; ( char* name )
; finds word _name_ and pushes it's xt
; to colon word being built at the moment 
; ( xt = execution token, void* code )
native ":push_word", push_word
	pop rdi
	call fetch_word_exec_addr

	jmp next

; ( char* name )
; creates name of new word (doesn't check if word exists yet )
; we will find word by this name in future
native ":create_word", create_word, _IMMEDIATE
	
	; init pointer to previous word
	mov rdx, [root_word] 
	mov rax, [new_root] ; rax - byte* 
	add rax, 8 ; space from previous word (can we decrease?)
	mov [rax], rdx ; *new_root = root_word
	add rax, 8 ; dq wh_last_word; p += 8

	; second stroke of the header
	mov byte [rax], 0 ; db 0
	inc rax ; p ++
	mov [new_root], rax ; update the pointer itself

	; init name of the word
	pop rdi ; str src
	push rdi ; copy str* on stack
	mov rsi, [new_root] ; str dst
	mov rdx, INPUT_BUFFER_SIZE
	call string_copy

	; probably, there's more sophisticated way to skip the name
	; but anyway
	pop rdi
	call string_length ; skip 'db "str", 0'
	inc rax ; \0 symbol
	mov rdx, [new_root] ; rdx = p
	add rax, rdx ; rax = p + sring_lenght
	mov [new_root], rax ; update the pointer (can be erased)
	
	; skip FLAGS
	mov byte[rax], 0  ; FLAGS = 0
	inc rax ; p++
	mov [new_root], rax ; update the pointer

	; next thing is xt_addr
	mov rdi, colon_word_string
	call fetch_word_exec_addr
	mov rdx, [new_root] ; rdx = p
	mov [rdx], rax ; *p = colon_xt
	add rdx, 8
	add [new_root], rdx

	jmp next

native ";", switch_to_enterpr, _IMMEDIATE
	mov qword [state], STATE_INTERPRET
	jmp next

colon "compiler", compiler
	

;;;;;;;MAIN_THING;;;;;;;;;;
colon "interpret", interpret
interp:	
	dq xt_print_welcome
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

colon "print_welcome", print_welcome
	dq xt_lit
	dq '~'
	dq xt_emit
	dq xt_lit
	dq '>'
	dq xt_emit
	dq xt_lit
	dq ' '
	dq xt_emit
	dq xt_leave

colon "print_no_word", print_no_word
	dq xt_get_input_buffer
	dq xt_print
	dq xt_lit
       	dq ':'
	dq xt_emit
	dq xt_get_no_such_word_buffer
	dq xt_print
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ( void* xt - )
native 'execute', execute
	pop rax
	mov w, rax
	jmp [rax]

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

; (char* - )	
native 'count', count
	pop rdi
	call string_length
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
	mov rsi, [root_word]
	call fetch_word_hdr_addr
	push rax
	jmp next

; ( char* buff - void* word_exec_code )
native 'find_word_xt', find_word_xt
	pop rdi
	mov rsi, [root_word]
	call fetch_word_exec_addr
	push rax
	jmp next

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
native '.S', show_stack
	mov rax, [inital_rsp]
	mov rcx, rsp
.loop:
	cmp rcx, rax
	jz next
	mov rdi, [rcx]
	add rcx, 8 ; ceil size
	push rax
	push rcx
	call print_int
	call print_newline
	pop rcx
	pop rax
	jmp .loop

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

;;;;;;;; STACK THINGS ;;;;;;;
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
; ( a b c - b c a )
native 'rot', rot
	pop rcx
	pop rdi
	pop rax
	push rdi
	push rcx
	push rax
	jmp next

;;;;;;;;;;;; LOGIC ;;;;;;;;;;;;;;;;;;;
native 'not', not
	pop rax
	test rax, rax
	jz .1
	xor rax, rax
	jmp .outro
.1:	mov rax, 1
.outro:	push rax
	jmp next

native '=', equals
	pop rax
	pop rcx
	cmp rax, rcx
	je .true
.false: xor rax, rax
	jmp .outro
.true:
	mov rax, 1
.outro:	push rax
	jmp next
	
;;;;;;;;;;;; ARIFMETIC ;;;;;;;;;;;;;;;
native '+', plus
	pop rax
	pop rcx
	add rax, rcx
	push rax
	jmp next

native "-", sub2
	pop rcx
	pop rax
	sub rax, rcx
	push rax
	jmp next

native '*', mul
	pop rax
	pop rcx
	imul rcx
	push rax
	jmp next

native '/', div
	xor edx, edx
	pop rcx
	pop rax
	idiv rcx
	push rax
	jmp next

native '%', mod
	xor edx, edx
	pop rcx
	pop rax
	idiv rcx
	push rdx
	jmp next

default_root:
	dq wh_mod

%endif ; WORDS_INC
