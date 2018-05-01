
global fetch_word_hdr_addr
extern root_word
%include "io_lib.inc"

%define  HDR_SIZE 9


; rdi - char* name
; ret rax: void* hdr_addr
fetch_word_hdr_addr:
.init:	mov rcx, [root_word]     ; rcx - current word's header

.loop:	
.check: lea  rsi, [rcx+HDR_SIZE] ; point to string #2
	call string_equals
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
	xor rax, rax
	ret	
