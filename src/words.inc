%ifndef WORDS_INC
%define WORDS_INC

%include "macro.inc"
%include "io_lib.inc"
%include "fmachine.inc"

%define _IMMEDIATE 0x01

section .rdata
compiler_off_string: db ";", 0
section .data
extern fetch_word_hdr_addr
extern fetch_word_exec_addr
extern take_xt_by_hdr
extern take_word_flag
extern for_each_word

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; to enter a colon word
native "docol", docol
	rpush pc
	mov rdi, '~'
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
	lea pc, [interp]
	mov rstack, rstack_buffer
	mov [inital_rsp], rsp ; let .S works
	mov rax, [default_root]
	mov [root_word], rax

	mov qword [last_command_in_new_word], new_words_creating_here
	jmp next	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
colon "test_colon", test_colon
	dq xt_42
	dq xt_43
	dq xt_leave

colon "test_colon_in_colon", test_colon_in_colon
	dq xt_test_colon
	dq xt_43
	dq xt_leave

;;;;;;;;;;; COMPILE THINGS ;;;;;;;;;;;;;
colon  ":", switch_to_compiler
	dq xt_to_compiler
	dq xt_print_welcome

	dq xt_get_input_buffer ; read name of the new word
	dq xt_read_word
	
	dq xt_get_input_buffer
	dq xt_create_word
	dq xt_compiler ; compiler loop
	dq xt_leave

native ":to_compiler", to_compiler
	mov qword [state], STATE_COMPILER
	mov rdx, [last_command_in_new_word] 
	add rdx, 8 ; We don't want to rewrite the last command!
	mov [building_word_addr], rdx
	jmp next

; pushes xt to colon word being built at the moment 
; ( xt )
native ":push_word", push_word
	pop rax ; rax = xt
	add qword [building_word_addr], 8 ; p += sizeof(dq)
	mov rdx, [building_word_addr] 
	mov [rdx], rax
	jmp next

; pushes tos to the end of the word being constructed
; ( value )
; help us to use lit word and place number after it as dq thing
; acually it's completely the same as push_word, so there's no anything

; ( char* name )
; creates name of new word (doesn't check if word exists yet )
; we will find word by this name in future
; it creates just a header of the colon word being created
native ":create_word", create_word, _IMMEDIATE
	
	; building word address has been already initialized
	mov rax, [building_word_addr] ; rax - byte* = &last_word_hdr
	mov qword[new_root], rax ; new_root->new_word hdr
	
	; init pointer to previous word
	mov rdx, [root_word]
	mov [rax], rdx ; *building_word_addr = root_word
	add rax, 8 ; dq wh_last_word; p += 8; p = new_hdr+8

	; second stroke of the header
	mov byte [rax], 0 ; db 0
	inc rax ; p ++; p = new_hdr+9
	mov [building_word_addr], rax ; update the pointer itself

	; init name of the word
	pop rdi  ; str src
	push rdi ; copy str* on stack
	mov rsi, [building_word_addr] ; str dst
	mov rdx, INPUT_BUFFER_SIZE    ; max size restriction
	call string_copy

	; probably, there's more sophisticated way to skip the name
	; that we have just initialized but anyway
	pop rdi
	call string_length ; skip 'db "str", 0'
	inc rax ; \0 symbol
	mov rdx, [building_word_addr] ; rdx = p
	add rax, rdx ; rax = p + sring_length  = hdr + 9 + len + 1
	mov [building_word_addr], rax ; update the pointer (can be erased)
	
	; skip FLAGS
	inc rax ; p++; p = hdr + 9 + len+1 +1
	mov byte[rax], 0  ; FLAGS = 0
	mov [building_word_addr], rax ; update the pointer

.coln:	; next thing is xt_addr
	mov rdx, [building_word_addr] ; rdx = p
	; first thing is dq i_docol! Not xt_docol
	mov qword[rdx], i_docol ; *p = dq i_docol
	; don't skip extra space
	mov [building_word_addr], rdx
	jmp next

;;;;;;;;;;; stop compiling ;;;
native ";", switch_to_enterpr, _IMMEDIATE
.xt_leave_word:
	mov rax, [building_word_addr]
	add rax, 8 ; last dq is 8 byte 
	mov qword [rax], xt_leave
	mov [last_command_in_new_word], rax
.update_root:
	mov rax, [new_root]
	mov [root_word], rax
	jmp [xt_leave]

;;;;;;;;;;; COMPILER LOOP ;;;;;;;;;;
;; compiler and interpeter loops are organazied
;; separately because I think it's more clear
colon "compiler", compiler
compiler:
	dq xt_lit, '~', xt_emit
	dq xt_print_welcome
	dq xt_get_input_buffer 
	dq xt_read_word
	dq xt_get_input_buffer
	dq xt_parse_int
	branch_if .num ; if it's num
	dq xt_drop ; parse int returns 2 args
	dq xt_get_input_buffer
.word:
	dq xt_find_word_hdr ; ( hdr )
	dq xt_dup
	branch_if .word_exists

.no_word:
	dq xt_print_no_word	
	dq xt_drop ; clear word hdr*
	branch compiler

.word_exists:
	;dq xt_get_input_buffer
	;dq xt_get_compiler_off_string
	;dq xt_string_equals
	;branch_if .compiler_off

	dq xt_dup ; duplicate hdr*
	
.is_immediate?:
	dq xt_is_immediate
	branch_if .perform_immediate

.push_new_word:
	dq xt_cfa ; get word's xt: ( hdr, xt )
	dq xt_push_word
	branch compiler

.perform_immediate:
	dq xt_cfa
	dq xt_execute
	branch compiler
.num:	
	; the numb's already on the TOS
	dq xt_push_next_word_xt
	dq xt_lit
	dq 42 ; it'll be skipped!
	dq xt_drop ; remove 42 from the stack
	dq xt_push_word ; push lit_word
	dq xt_push_word ; push number as word
			; the numer will be dq thing
			; placed right after lit to let it work
	branch compiler
.compiler_off:
	dq xt_drop ; remove excess 6293463 thing from the stack
	dq xt_switch_to_enterpr
	dq xt_leave

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

;;;;;;;;THINGS PRINTING THINGS;;;;;;;;;;;;;;;
colon "print_welcome", print_welcome
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

print_word:
	push rdi ; store str*
	call print_string
	mov rdi, ' '
	call print_char
	pop rdi ; restore str*
	call string_length
	add rdi, rax ; check if word is immediate
	inc rdi
	mov rdi, [rdi]
	add rdi, '0'
	call print_char
	call print_newline
	ret

native "ls", list_all_words
	lea rdi, [print_word]
	mov rsi, [root_word]
	call for_each_word
	jmp next

;;;;;;;; BRANCHES ;;;;;;;;;
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

colon "0branch", zero_branch
	dq xt_not
	dq xt_branch_if
	dq xt_leave

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
	jmp next

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

;;;;;;;; HARDCODE ADDRS THINGS ;;;;;
native 'get_input_buffer', get_input_buffer
	push input_buffer
	jmp next

native 'get_compiler_off_string', get_compiler_off_string
	push compiler_off_string
	jmp next

; pushes the xt of the next dq thing onto the stack
native 'push_next_word_xt', push_next_word_xt
	mov rax, [pc]
	push rax
	jmp next
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ( char* - )
; read a word from stdin and store it into the addr
native 'read_word', read_word
	pop rdi
	mov rsi, INPUT_BUFFER_SIZE
	call read_word
	jmp next

; (char* -- )
; read a char and store it
native 'readc@', readc@
	call read_char
	pop rdi
	mov byte[rdi], al
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
	dq xt_leave

; ( char* buff - void* word_hdr)
native 'find_word_hdr', find_word_hdr
	pop rdi
	mov rsi, [root_word]
	call fetch_word_hdr_addr
	push rax
	jmp next

; THE SAME AS WORD BELOW
; ( char* buff - void* word_exec_code )
native 'find_word_xt', find_word_xt
	pop rdi
	mov rsi, [root_word]
	call fetch_word_exec_addr
	push rax
	jmp next

; ( word hdr - immediate?: boolean ) 
native 'is_word_immediate', is_immediate
	pop rdi
	call take_word_flag
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
native '42', 42, 
	mov rdi, 42
	call print_char
	call print_newline
	jmp next

native '43', 43
	mov rdi, 43
	call print_char
	call print_newline
	jmp next

;;;;;;;; MEMORY THINGS ;;;;;;;;

; fetch dq from memory
; ( addr - value )
native '@', @
	pop rax
	push qword [rax]
	jmp next

; fetch byte from memory
; ( addr - byte )
native 'c@', c@
	pop rdi
	xor rax, rax
	mov al, byte[rdi]
	push rax
	jmp next

; store dq value by addres
; ( value, addr -- )
native '!', store_dq
	pop rax
	pop rdi
	mov [rax], rdi
	jmp next

; store byte value by addres
; ( byte, addr -- )
native 'c!', store_byte
	pop rax
	pop rdi
	mov [rax], byte dil
	jmp next

; from data stack into return stack
native '>r', r_to 
	pop rax
	rpush rax
	jmp next

; from return stack into data stack
native 'r>', r_from
	rpop rax
	push rax
	jmp next

native 'r@', r_fetch
	push qword [rstack]
	jmp next

; take the address of the end of the heap''
native 'dp', heap_end
	mov rax, dp
	push rax
	jmp next

; push input buffer addres
native 'inbuf', inbuf
	mov rax, input_buffer
	push rax
	jmp next
	
; push next dq thing onto the stack
native 'lit', lit
	push qword[pc] ; push next ceil
	add pc, 8 ; next ceil's data, not a command
	jmp next
	
; pushes an adrres of an adress of the header of the last word
native 'last_word', last_word
	mov rax, root_word
	; As for me, it better to use mov rax, [root_word] instead
	; but it worsk such way to be compatibility with the teacher's stdlib.frt
	push rax
	jmp next

; points to the last word currently being defined 
native 'here', here, _IMMEDIATE
	mov rax, [building_word_addr] ; test me!!
	push rax
	jmp next

; add x to the word-ceil currently being built 
; ( x -- )
native ',', comma
	mov rax, [building_word_addr]
	pop qword[rax]
	add qword[building_word_addr], 8
	jmp next

;;;;;;;; COMPATIBILITY  ;;;;;;;;;;
colon "'", give_me_xt_of_the_next_word
	dq xt_get_input_buffer
	dq xt_read_word
	dq xt_get_input_buffer
	dq xt_find_word_xt
	dq xt_leave

; convert header to xt
; ( hdr* - xt )
native 'cfa', cfa
	pop rdi
	call take_xt_by_hdr
	push rax
	jmp next

;;;;;;;; STACK THINGS ;;;;;;;;;;
native 'dup', dup
	push qword[rsp]
	jmp next

native 'drop', drop
	add rsp, 8
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

;;;;;;;;;;;; STRINGS ;;;;;;;;;;;;;;;;;
; ( char* s1, char* s2 - equals? )
native 'string_equals', string_equals
	pop rdi
	pop rsi
	call string_equals
	push rax
	jmp next

;;;;;;;;;;;; LOGIC ;;;;;;;;;;;;;;;;;;;
native '<', less
	pop rax
	pop rdx
	cmp rax, rdx
	jle .more
	push 1
	jmp next
.more:
	push 0
	jmp next

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

native 'and', bit_and
; ( a,b - a & b )
native 'land', logical_and
	pop rax
	pop rdx
	and rax, rdx
	push rax
	jmp next
native 'or', bit_bor
native 'lor', logical_or
	pop rax
	pop rdx
	or rax, rdx
	push rax
	jmp next

;;;;;;;;;;;; ARIFMETIC ;;;;;;;;;;;;;;;
native '+', plus
	pop rax
	pop rcx
	add rax, rcx
	push rax
	jmp next

native '-', substract
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

default_root: dq wh_mod
section .bss
new_words_creating_here: resb NEW_WORDS_MEMORY_SIZE

%endif ; WORDS_INC
