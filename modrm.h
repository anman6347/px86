#ifndef MODRM_H_
#define MODRM_H_

#include <stdint.h>

#include "emulator.h"

typedef struct {
    uint8_t mod;

    /* opecode と reg_index は別名で同じもの */
    union {
        uint8_t opecode;
        uint8_t reg_index;
    };

    uint8_t rm;

    /* SIB が必要な mod/rm の組み合わせに使う */
    uint8_t sib;

    union {
        int8_t disp8;  // disp8 は符号付き整数値
        uint32_t disp32;
    };

} ModRM;

/*  ModR/M, SIB, ディスプレースメントを解析する.
    emu から ModR/M, SIB, ディスプレースメントを読み取って modrm にセットする.
*/
void parse_modrm(Emulator* emu, ModRM* modrm);

/* rm32 のレジスタまたはメモリの 32bit 値を取得する */
uint32_t get_rm32(Emulator* emu, ModRM* modrm);

/* rm32 のレジスタまたはメモリに 32bit 値を設定する */
void set_rm32(Emulator* emu, ModRM* modrm, uint32_t value);

/* r32 のレジスタの 32bit 値を取得する */
uint32_t get_r32(Emulator* emu, ModRM* modrm);

/* r32 のレジスタに 32bit 値を設定する */
void set_r32(Emulator* emu, ModRM* modrm, uint32_t value);

/* ModR/M の内容に基づき、実効アドレスを計算する */
uint32_t calc_memory_address(Emulator* emu, ModRM* modrm);

#endif