#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE (1024 * 1024)

enum Register { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, REGISTERS_COUNT };
const char *registers_name[] = {"EAX", "ECX", "EDX", "EBX",
                                "ESP", "EBP", "ESI", "EDI"};

typedef struct {
    /* 汎用レジスタ 8 個*/
    uint32_t registers[REGISTERS_COUNT];

    /* EFLAGSレジスタ */
    uint32_t eflags;

    /* メモリ（バイト列） */
    uint8_t *memory;

    /* プログラムカウンタ */
    uint32_t eip;

} Emulator;

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
    for(i = 0; i < REGISTERS_COUNT; i++) {
        printf("%s = %08x\n", registers_name[i], emu->registers[i]);
    }
    printf("EIP = %08x\n", emu->eip);
}

/* memory 配列の eip が指す番地から 8 ビットの符号無し整数値を取得し、32
 * ビットで返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code8(Emulator *emu, int index) {
    return emu->memory[emu->eip + index];
}

/* memory 配列の eip が指す番地から 8 ビットの符号付き整数値を取得し、32
 * ビットで返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code8(Emulator *emu, int index) {
    return (int8_t)emu->memory[emu->eip + index];
}

/* memory 配列の eip が指す番地から 32 ビットの符号無し整数値を取得し、32
 * ビットで返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code32(Emulator *emu, int index) {
    int i;
    uint32_t ret = 0;

    /* リトルエンディアンでメモリの値を取得する */
    for(i = 0; i < 4; i++) {
        ret |= get_code8(emu, index + i) << (i * 8);  // 8 ビット分左にずらす
    }
    return ret;
}

/* memory 配列の eip が指す番地から 32 ビットの符号付き整数値を取得し、32
 * ビットで返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code32(Emulator *emu, int index) {
    return (int32_t)get_code32(emu, index);
}

/* 汎用レジスタに 32 ビットの即値をコピー */
void mov_r32_imm32(Emulator *emu) {
    uint8_t reg = get_code8(emu, 0) - 0xB8;  //レジスタの番号取得
    uint32_t value = get_code32(emu, 1);     //代入する値を取得
    emu->registers[reg] = value;
    emu->eip += 5;
}

/* 1 バイトの符号付き整数値を読み取り、次の命令の番地を基点としてジャンプする */
void short_jump(Emulator *emu) {
    int8_t diff = get_sign_code8(emu, 1);
    emu->eip += (diff + 2);
}

/* 4 バイトの符号付き整数値を読み取り、次の命令の番地を基点としてジャンプする */
void near_jump(Emulator *emu) {
    int32_t diff = get_sign_code32(emu, 1);
    emu->eip += (diff + 5);
}

/* 関数ポインタテーブルの作成とその初期化 */
typedef void instruction_func_t(Emulator *);
instruction_func_t *instructions[256];
void init_instructions() {
    int i;
    /* すべてのアドレスを 0 で初期化 */
    memset(instructions, 0, sizeof(instructions));

    /* 0xB8 から 8 つを mov_r32_imm32 に、0xE9 を near_jump に、0xEB を
     * short_jump に */
    for(i = 0; i < 8; i++) {
        instructions[0xB8 + i] = mov_r32_imm32;
    }
    instructions[0xE9] = near_jump;
    instructions[0xEB] = short_jump;
}

int main(int argc, char *argv[]) {
    FILE *binary;
    Emulator *emu;

    if(argc != 2) {
        printf("usage: px86 filename\n");
        return 1;
    }

    /* EIP, ESP が 0x7c00 の状態のエミュレータを作る */
    emu = create_emu(MEMORY_SIZE, 0x7c00, 0x7c00);

    binary = fopen(argv[1], "rb");
    if(binary == NULL) {
        printf("%s ファイルが開けません\n", argv[1]);
        return 1;
    }

    /* 0x7c00 番地から機械語ファイルを読み込む（最大 512 バイト） */
    fread(emu->memory + 0x7c00, 1, 0x200, binary);
    fclose(binary);

    init_instructions();

    while(emu->eip < MEMORY_SIZE) {
        uint8_t code = get_code8(emu, 0);
        /* 実装していないコードの処理 */
        if(instructions[code] == NULL) {
            printf("\n\nnot Implemented: %x\n", code);
            break;
        }
        instructions[code](emu);
        if(emu->eip == 0x00) {
            printf("\n\nend of program.\n\n");
            break;
        }
    }
    dump_registers(emu);
    destroy_emu(emu);
    return 0;
}