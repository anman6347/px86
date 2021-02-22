TARGET = px86
BIN = helloworld.bin
PROGRAM = $(TARGET) $(BIN)
SRCS = main.c helloworld.asm
OBJS = main.o

NASM = ../tolset_p86/tolset_p86/z_tools/nasm.exe
LD = ld
CC = clang-10
CFLAGS = -Wall

all : $(PROGRAM)

$(TARGET): $(OBJS)
	$(CC) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.bin : %.asm
	$(NASM) -f bin -o $@ $<

clean :
	rm $(PROGRAM) $(OBJS)

test: 
	./$(TARGET) $(BIN)

.PHONY : all clean test
