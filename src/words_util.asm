
global fetch_word_hdr_addr
global fetch_word_exec_addr

extern root_word
%include "io_lib.inc"

%define  HDR_SIZE  9
%define  FLAG_SIZE 1

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
; ret rax: void* instructions
; ret rax: zero if there's no such word
fetch_word_exec_addr:
	call fetch_word_hdr_addr
	test rax, rax
	jz .notFound
.ok:	
	add rax, HDR_SIZE ; HDR_SIZE - bytes before the string
	mov rcx, rax ; rcx <- words_name addr
	mov rdi, rax 
	push rcx
	call string_length
	inc rax ; \0 byte taked into account
	pop rcx
	mov rax, [rax + rcx + FLAG_SIZE] ; 
.found:
.notFound:
	ret


