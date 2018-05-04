%ifndef FORTH_MACHINE_INC
%define FORTH_MACHINE_INC

%define pc r15
%define w  r14
%define rstack r13

%define INPUT_BUFFER_SIZE 512
%define RETURN_STACK_SIZE 512

%define STATE_COMPILER 	0x1
%define STATE_INTERPRET 0x0

global input_buffer

section .bss

input_buffer: resq INPUT_BUFFER_SIZE
resq RETURN_STACK_SIZE ; rstack grows down

rstack_buffer: resq 1
inital_rsp: resq 1

state: resq 1 ; compiler or interpret
root_word: resq 1 ; root word addr
new_root:  resq 1 ; helps us to add new word
program_stub: resq 1;

section .data
%include "words.inc"
	
%endif ; FORTH_MACHINE_INC
