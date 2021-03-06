TARGET = px86
BIN = test.bin
SRCS = main.c instruction.c modrm.c emulator_function.c emulator.h emulator_function.h instruction.h modrm.h
OBJS = main.o instruction.o modrm.o emulator_function.o

NASM = ../tolset_p86/tolset_p86/z_tools/nasm.exe
LD = ld
CC = clang-10
CFLAGS = -Wall

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm $(TARGET) $(OBJS)

test: 
	./$(TARGET) $(BIN)

.PHONY : clean test
