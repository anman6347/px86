TARGET = px86
BIN = helloworld.bin
PROGRAM = $(TARGET) $(BIN)
SRCS = main.c instruction.c modrm.c emulator_function.c emulator.h emulator_function.h instruction.h modrm.h helloworld.asm
OBJS = main.o instruction.o modrm.o emulator_function.o

NASM = ../tolset_p86/tolset_p86/z_tools/nasm.exe
LD = ld
CC = clang-10
CFLAGS = -Wall

all : $(PROGRAM)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.bin : %.asm
	$(NASM) -f bin -o $@ $<

clean :
	rm $(PROGRAM) $(OBJS)

test: 
	./$(TARGET) $(BIN)

.PHONY : all clean test
