%ifndef FORTH_MACHINE_INC
%define FORTH_MACHINE_INC

%define pc r15
%define w  r14
%define rstack r13

%define INPUT_BUFFER_SIZE 512
%define RETURN_STACK_SIZE 512

global input_buffer

section .bss

input_buffer: resq INPUT_BUFFER_SIZE
resq RETURN_STACK_SIZE ; rstack grows down
rstack_buffer: resq 1
inital_rsp: resq 1

section .data
%include "words.inc"
	
%endif ; FORTH_MACHINE_INC
