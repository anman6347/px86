TARGET = px86
BIN = helloworld.bin
SRCS = main.c instruction.c modrm.c emulator_function.c io.c emulator.h emulator_function.h instruction.h modrm.h io.h
OBJS = main.o instruction.o modrm.o emulator_function.o io.o

NASM = ../tolset_p86/tolset_p86/z_tools/nasm.exe
LD = ld
CC = clang-10
CFLAGS = -Wall

all: $(TARGET) $(BIN)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

$(BIN): helloworld.asm
	$(NASM) -f bin -o $@ $<

clean :
	rm $(TARGET) $(OBJS) $(BIN)

test: 
	./$(TARGET) -q $(BIN)

.PHONY : all clean test
