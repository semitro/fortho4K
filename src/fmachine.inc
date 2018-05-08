%ifndef FORTH_MACHINE_INC
%define FORTH_MACHINE_INC

%define pc r15
%define w  r14
%define rstack r13

%define INPUT_BUFFER_SIZE 512
%define RETURN_STACK_SIZE 512

%define NEW_WORDS_MEMORY_SIZE 65536
; It's not used in this implementation of Forth
; but it's still here because it migth be usefull in the future
%define STATE_COMPILER 	0x1
%define STATE_INTERPRET 0x0

global input_buffer

section .bss

input_buffer: resq INPUT_BUFFER_SIZE
resq RETURN_STACK_SIZE ; rstack grows down

rstack_buffer: resq 1
inital_rsp: resq 1

; it's written but not actually used
state: resq 1 ; compiler or interpret

root_word: resq 1 ; root word addr
building_word_addr:  resq 1 ; last addr of the new word
new_root: resq 1  ; It's a  pointer to the header of the new word
last_command_in_new_word: resq 1 ; it's a pointer to the end of the new word.
; it's easier and cheaper to remember than to compute
section .data
%include "words.inc"
	
%endif ; FORTH_MACHINE_INC
