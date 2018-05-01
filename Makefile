CC=nasm
OUT_NAME=forto4ka
CFLAGS= -f elf64 -Isrc/

SRC=src/main.asm


all: 
	$(CC) $(CFLAGS) $(SRC)

clean: 
	$(RM) $(OUT_NAME)

