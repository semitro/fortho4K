%ifndef FORTH_MACHINE_INC
%define FORTH_MACHINE_INC

%define PC r15
%define W  r14

%define INPUT_BUFFER_SIZE 512
global input_buffer

section .bss
input_buffer: resq INPUT_BUFFER_SIZE

section .data
%include "words.inc"
	
%endif ; FORTH_MACHINE_INC
