#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator.h"
#include "emulator_function.h"
#include "instruction.h"

#define MEMORY_SIZE (1024 * 1024)

const char *registers_name[] = {"EAX", "ECX", "EDX", "EBX",
                                "ESP", "EBP", "ESI", "EDI"};

/* エミュレータを作成 */
static Emulator *create_emu(size_t size, uint32_t eip, uint32_t esp) {
    /* 作成したエミュレータを emu が指す */
    Emulator *emu = (Emulator *)malloc(sizeof(Emulator));

    /* memory に size分割り当てる */
    emu->memory = (uint8_t *)malloc(size);

    /* すべての汎用レジスタの初期値を 0 にする */
    memset(emu->registers, 0, sizeof(emu->registers));

    /* PCレジスタの初期値を指定さ入れたものにする */
    emu->eip = eip;
    emu->registers[ESP] = esp;

    return emu;
}

/* エミュレータの破棄 */
static void destroy_emu(Emulator *emu) {
    free(emu->memory);
    free(emu);
}

/* 汎用レジスタとプログラムカウンタの値を標準出力 */
static void dump_registers(Emulator *emu) {
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++) {
        printf("%s = %08x\n", registers_name[i], emu->registers[i]);
    }
    printf("EIP = %08x\n", emu->eip);
}

int opt_remove_at(int argc, char *argv[], int index) {
    if (index < 0 || argc <= index) {
        return argc;
    } else {
        int i = index;
        for (; i < argc - 1; i++) {
            argv[i] = argv[i + 1];
        }
        argv[i] = NULL;
        return argc - 1;
    }
}

int main(int argc, char *argv[]) {
    FILE *binary;
    Emulator *emu;

    int i = 1;
    int quiet = 0;

    while (i < argc) {
        if (strcmp(argv[i], "-q") == 0) {
            quiet = 1;
            /* -q 以降の引数をひとつずらす */
            argc = opt_remove_at(argc, argv, i);
        } else {
            i++;
        }
    }

    if (argc != 2) {
        printf("usage: px86 filename\n");
        return 1;
    }

    /* EIP, ESP が 0x7c00 の状態のエミュレータを作る */
    emu = create_emu(MEMORY_SIZE, 0x7c00, 0x7c00);

    binary = fopen(argv[1], "rb");
    if (binary == NULL) {
        printf("%s ファイルが開けません\n", argv[1]);
        return 1;
    }

    /* 0x7c00 番地から機械語ファイルを読み込む（最大 512 バイト） */
    fread(emu->memory + 0x7c00, 1, 0x200, binary);
    fclose(binary);

    init_instructions();

    while (emu->eip < MEMORY_SIZE) {
        uint8_t code = get_code8(emu, 0);
        /* 現在のプログラムカウンタと実行されるバイナリを出力する */
        if (!quiet) {
            printf("EIP = %x, Code = %02x\n", emu->eip, code);
        }
        /* 実装していないコードの処理 */
        if (instructions[code] == NULL) {
            printf("\n\nnot Implemented: %x\n", code);
            break;
        }
        instructions[code](emu);
        if (emu->eip == 0x00) {
            printf("\n\nend of program.\n\n");
            break;
        }
    }
    dump_registers(emu);
    destroy_emu(emu);
    return 0;
}