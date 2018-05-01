ASM=nasm
OUT_NAME=forto4ka
ASMFLAGS= -f elf64 -Isrc/
LD=ld

all: $(OUT_NAME) 

src/io_lib.o: src/io_lib.asm
	$(ASM) $(ASMFLAGS) src/io_lib.asm

src/words_util.o: src/words.inc
	$(ASM) $(ASMFLAGS) src/words_util.asm

src/main.o: src/main.asm src/io_lib.inc src/fmachine.inc
	$(ASM) $(ASMFLAGS) src/main.asm 

$(OUT_NAME): src/io_lib.o src/main.o src/words_util.o
	$(LD) $(LFAGS) -o $(OUT_NAME) src/io_lib.o src/main.o src/words_util.o


clean: 
	$(RM) src/$(OUT_NAME) $(RM) src/*.o

