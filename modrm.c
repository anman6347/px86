#include "modrm.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator_function.h"

void parse_modrm(Emulator* emu, ModRM* modrm) {
    uint8_t code;
    memset(modrm, 0, sizeof(ModRM));  //全て 0 で初期化

    code = get_code8(emu, 0);
    modrm->mod = ((code & 0xC0) >> 6);
    modrm->opecode = ((code & 0x38) >> 3);
    modrm->rm = code & 0x07;

    emu->eip += 1;

    /* mod が 11 でなく、rm が 100 のとき SIB を読み取る*/
    if(modrm->mod != 3 && modrm->rm == 4) {
        modrm->sib = get_code8(emu, 0);
        emu->eip += 1;
    }

    /* ディスプレースメントの指定がある場合に読み取る */
    if((modrm->mod == 0 && modrm->rm == 5) || modrm->mod == 2) {
        modrm->disp32 = get_sign_code32(emu, 0);
        emu->eip += 4;
    } else if(modrm->mod == 1) {
        modrm->disp8 = get_sign_code8(emu, 0);
        emu->eip += 1;
    }
}

uint32_t get_rm32(Emulator* emu, ModRM* modrm) {
    if(modrm->mod == 3) {
        return get_register32(emu, modrm->rm);
    } else {
        uint32_t address = calc_memory_address(emu, modrm);
        return get_memory32(emu, address);
    }
}

void set_rm32(Emulator* emu, ModRM* modrm, uint32_t value) {
    if(modrm->mod == 3) { /* mod = 11 のとき */
        set_register32(emu, modrm->rm, value);
    } else {
        uint32_t address = calc_memory_address(emu, modrm);
        set_memory32(emu, address, value);
    }
}

uint32_t get_r32(Emulator* emu, ModRM* modrm) {
    return get_register32(emu, modrm->reg_index);
}

void set_r32(Emulator* emu, ModRM* modrm, uint32_t value) {
    set_register32(emu, modrm->reg_index, value);
}

uint32_t calc_memory_address(Emulator* emu, ModRM* modrm) {
    if(modrm->mod == 0) {
        if(modrm->rm == 4) {
            printf("not implemented ModRM mod = 0, rm = 4\n");
            exit(0);
        } else if(modrm->rm == 5) {
            return modrm->disp32;
        } else {
            return get_register32(emu, modrm->rm);
        }
    } else if(modrm->mod == 1) {
        if(modrm->rm == 4) {
            printf("not implemented ModRM mod = 1, rm = 4\n");
            exit(0);
        } else {
            return get_register32(emu, modrm->rm) + modrm->disp8;
        }
    } else if(modrm->mod == 2) {
        if(modrm->rm == 4) {
            printf("not implemented ModRM mod = 2, rm = 4\n");
            exit(0);
        } else {
            return get_register32(emu, modrm->rm) + modrm->disp32;
        }
    } else {
        printf("not implemented ModRM mod = 3\n");
        exit(0);
    }
}