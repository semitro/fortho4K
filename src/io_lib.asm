; Some usefull utility functions

global string_length
global print_string
global print_newline
global print_char
global print_int
global print_uint
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global exit
global string_copy

section .text

string_length:
    xor rax, rax
    mov r10, 0 ; \0 char. This idea's silly. 
	       ; I just thought that \0 symbol has code 0xA
	       ; However we can easy mark any symbol as end of a string
    
._getChars:
    mov rcx, 8
    mov rdx, [rdi] ;rdx - tmp register containing the chars
._shift:    
    cmp dl, r10b ; condition: is it '\0'?
    jz ._ret
    inc rax
    sar rdx, 8
    loop ._shift 
    ;get next 8 chars from RAM
    add rdi, 8 ; Восемь адресов спустя лежит следующий кусок строки
    jmp ._getChars
._ret:
    ret

;Args: rdi - pointer of null-terminated string 
print_string:
    mov rsi, rdi ; buffer pointer
    call string_length
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret

;ch_b1: db '1234567890987654321hehehey ', 0
;char_buffer:
;	db 0,'   ENDENDEND',0,0,0
;char_b2: 
;	db 'KRASOTA'
;The ASCII code of the char should be in rdi 

section .text

print_newline:
	mov rdi, 10
; Instead of using 'call print_char' 
; Or tail-call optimization
; We just allow the CPU continue its work,
; So prnt_char'll be invoked by itself
; Don't separate these two functions!
; They must follow one another.

print_char:
    mov rsi, rsp

.print_char_tail: ; Marked to use tail-call optimization in
		  ; print_newline function
    mov rax, 1  ; Sys call number
    		; The symbol must be in the RAM
    push rdi
    mov rdi, 1   ; descriptor
    mov rsi, rsp ; The addr of the sybmol
    mov rdx, 1   ; Number of chars
    syscall
    pop rdi
    ret


;print_newline:
    ;mov rdi, 10 ; The code of \n
    ; We do not use calling the function. 
    ; Instead of this we prefer tail-call optimization
    ;jmp print_char.print_char_tail

print_int:
	test rdi, rdi
	jns .positive
	push rdi
        mov rdi, '-'
        call print_char
	pop rdi
	neg rdi	
.positive:
         ; Then the code will flow into the print_uint function
	 ; And our digit will be shown

print_uint:
    push r12
    mov rax, rdi ; the number to print
    mov r10, 10
    xor r12, r12 ; the number of chars
.next_digit:
    xor rdx, rdx
    div r10 ; edx - remainder
    add edx, 0x30 ; convert the digit to an ascii-code
    inc r12 ; It's going to help us make a loop
    push rdx
    test eax, eax
    jnz .next_digit
.print:
    pop  rdi
    call print_char
    dec r12
    jnz .print
    pop r12
    ret
; rdi - one string pointer
; rsi - two string pointer
string_equals:
.next_char:
    mov al, byte[rdi]
    cmp al, byte[rsi]
    jnz .not_equals
    inc rsi
    inc rdi 
    test al, al
    jnz .next_char
    mov rax, 1
    ret
.not_equals:
    mov rax, 0
    ret

; READ_READ_READ_READ_READ_READ_READ_READ_READ_READ_READ_READ
; Remaind to myself: don't use rcx!
read_char:
    mov rax, 0 ; sys_read 
    mov rdi, 0 ; input stream
    mov rdx, 1 ; Number of chars
       
    lea rsi, [rsp - 1] ; The char will be there
    syscall
    test rax, rax
    jz .ret
    xor rax, rax
    mov al, byte[rsi] ; return value
.ret:
    ret 


; This code is bad
read_word:
    push r12
    push r15
    push r10

    xor r10, r10 ; It's the flag to skip the first spaces

    push rdi     ; Save the pointer to the buffer
    xor rdx, rdx ; Symbols counter 
    mov r12, rdi ; Calle-save addr of the current ceil
    mov r15, rsi ; Buffer limit

.next_char:

    test r15, r15   ; We're tracing overload of the buffer
    jz .buffer_overload
    push rdx
    call read_char
    pop rdx
    test rax,rax  ; Is it \0?
    jz .null
    cmp al, 0x20
    jz .white
    cmp al, 0x9
    jz .white
    cmp al, 0xA
    jz .white
    inc r10       ; this indicadets that we've skipped the first whitespaces (not zero is enough)
    mov [r12], al ; We need to transfer every next char in the buffer
    inc r12       ; Shift the pointer to write next char
    inc rdx       ; Increase the counter
    dec r15 	  ; Buffer_limit
   jmp .next_char

.buffer_overload:
    pop rax
    xor rax, rax ; 0 in rax is the error code
    xor rdx, rdx
    jmp .ret
.white:
    test r10, r10
    jz .next_char
    pop rax ; recover the address of the buffer
.ret:
    pop r10
    pop r15
    mov [r12], byte 0
    pop r12
    ret
.null:
     pop rax
jmp .ret


; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
   xor rax, rax ; rax accumulates the answer

   xor r8, r8 ; r8 accumulates the length
   xor rsi, rsi
   mov r10, 10  ; We're going to mul by this
.next_char:
	
    mov sil, byte[rdi] ; rsi - current symbol
    
.is_a_numeral:   
    cmp sil, 0x30 ; We're checking if the current char in set of 0..9 
    js	.not_a_number ; Above ['0'
    cmp sil, 0x40 
    jns .not_a_number ; Beneath '9']
.yes:
    mul r10 ; We're going to use a formula rax*10 + number
    inc r8 ; If we've got a numeral increase numerals quantity
    sub rsi, 0x30 ; We need to add a number, not a char
    add rax, rsi ; Add the current number
    inc rdi ; Increse the pointer
    jmp .next_char
.not_a_number:
    mov rdx, r8
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
	mov al, byte[rdi] ; check the sign
	cmp al, 0x2D  ; is first symbol '-'
	jnz parse_uint
        inc rdi        ; negative number
	call parse_uint
 	neg rax
	inc rdx
	ret

exit:
	mov rax, 60
	syscall


; rdi - a pointer to the null-terimated string
; rsi - a pointer to the destination string
; rdx - the size of destination buffer
string_copy:
    push rsi     ; keep the pointer to return it in normal case
.next_char:
    cmp byte[rdi], 0 ; if the next char is \0 then we need to copy it
		     ; taking into consideration that the size 
		     ; of the buffer allows it
    jz .last_symbol
    test rdx, rdx ; The buffer overloading tracking
    jz .overflow

    mov al, byte[rdi]
    dec rdx
    mov byte[rsi], al
    inc rdi
    inc rsi
    jmp .next_char
.overflow:
    pop rax
    mov rax, 0 ; Returns 0 if there is the overflow
    ret
.last_symbol:
    test rdx, rdx ; Does the size allow us to write the char?
    jz .overflow  ; If not that it is the overflow
    mov byte[rsi], 0 ; If yes we should write \0 char
    pop rax ; Returns the addr of the dest if success 
    ret

