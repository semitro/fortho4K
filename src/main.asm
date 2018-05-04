global _start
global next

%include "io_lib.inc"
%include "fmachine.inc"

section .rodata
no_such_word_str:
db "No such word", 10, 0

section .text
next:
	mov w, [pc]  ; word that is going to be executed
	add pc, 8  ; pc  -> next word
	jmp [w]    ; rip -> xt_impl

_start:
	jmp i_init
