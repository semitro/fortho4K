global fetch_word_hdr_addr
global fetch_word_exec_addr
global take_xt_by_hdr
global for_each_word

extern root_word

%include "io_lib.inc"

%define  HDR_SIZE  9
%define  FLAG_SIZE 1

; rdi - function (rdi* str)
; rsi - hdr* root_node
for_each_word:
	mov rcx, rsi
.loop:
	lea rsi, [rcx + HDR_SIZE]
	push rcx
	push rdi
	mov rax, rdi
	mov rdi, rsi
	call rax
	pop rdi
	pop rcx

.to_next:
.if_we_can:
	mov rax, [rcx] ; have we got the next word?
	test rax, rax
	jz .end
	mov rcx, [rcx]
	jmp .loop
.end:
	ret

; rdi - char* name
; rsi - hdr* root_node
; ret rax: void* hdr_addr
fetch_word_hdr_addr:
	mov rcx, rsi
.init:	;mov rcx, [root_word]     ; rcx - current word's header

.loop:	
.check: lea  rsi, [rcx+HDR_SIZE] ; point to string #2
	push rdi
	call string_equals
	pop rdi
	test rax, rax
	jz .next_word
.found:	
	mov rax, rcx
	ret

.next_word:
.can_we:
 	mov rax, [rcx] ; test if next word exists
	test rax, rax
	jz .notFound
	mov rcx, [rcx]
	jmp .loop

.notFound:
	xor rax, rax ; it's not neccessary!!
	ret	

; rdi - char* name
; rsi - hdr* root_node
; ret rax: void* instructions
; ret rax: zero if there's no such word
fetch_word_exec_addr:
	call fetch_word_hdr_addr
	test rax, rax
	jnz skip_hdr
	ret
.ok:	
skip_hdr:
	add rax, HDR_SIZE ; HDR_SIZE - bytes before the string
	mov rcx, rax ; rcx <- words_name addr
	mov rdi, rax 
	push rcx
	call string_length
	inc rax ; \0 byte taked into account
	pop rcx
	lea rax, [rax + rcx + FLAG_SIZE] ; 
.found:
	ret

; take the exec addr knowing hdr addr
; rdi - hdr*
; rax: void* xt
take_xt_by_hdr:
	mov rax, rdi
	jmp skip_hdr
	; let fetch_word_exec_addr function work

; rdi - char * name
; rsi - hdr * root_node
skip_entire_word:
	call fetch_word_exec_addr
	jz .notFound
.found:	add rax, 9 ; skip FLAG and exec addr
.notFound:
	ret
