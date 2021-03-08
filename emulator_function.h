#ifndef EMULATOR_FUNCTION_H_
#define EMULATOR_FUNCTION_H_

#include <stdint.h>

#include "emulator.h"

/* EFLAGS のビットフラグ */
#define CARRY_FLAG (1)
#define ZERO_FLAG (1 << 6)
#define SIGN_FLAG (1 << 7)
#define OVERFLOW_FLAG (1 << 11)

/* memory 配列の eip が指す番地から 8bit の符号無し整数値を取得し、32bit で返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code8(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 8bit の符号付き整数値を取得し、32bit で返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code8(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 32bit の符号無し整数値を取得し、32bit で返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code32(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 32bit の符号付き整数値を取得し、32bit で返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code32(Emulator *emu, int index);

/* index 番目の 8bit 汎用レジスタの値を取得する */
uint8_t get_register8(Emulator *emu, int index);

/* index 番目の 8bit 汎用レジスタに値を設定する */
void set_register8(Emulator *emu, int index, uint8_t value);

/* index 番目の 32bit 汎用レジスタの値を取得する */
uint32_t get_register32(Emulator *emu, int index);

/* index 番目の 32bit 汎用レジスタに値を設定する */
void set_register32(Emulator *emu, int index, uint32_t value);

/* メモリの index 番地の 8bit 値を取得する */
uint32_t get_memory8(Emulator *emu, uint32_t address);

/* メモリの index 番地に 8bit 値を設定する */
void set_memory8(Emulator *emu, uint32_t address, uint32_t value);

/* メモリの index 番地の 32bit 値を取得する */
uint32_t get_memory32(Emulator *emu, uint32_t address);

/* メモリの index 番地に 32bit 値を設定する */
void set_memory32(Emulator *emu, uint32_t address, uint32_t value);

/* スタックに32bit値を積む */
void push32(Emulator *emu, uint32_t value);

/* スタックから32bit値を取りだす */
uint32_t pop32(Emulator *emu);

/* EFLAG の各フラグ設定用関数 */
void set_carry(Emulator *emu, int is_carry);
void set_zero(Emulator *emu, int is_zero);
void set_sign(Emulator *emu, int is_sign);
void set_overflow(Emulator *emu, int is_overflow);

/* EFLAG の各フラグ取得関数 */
int is_carry(Emulator *emu);
int is_zero(Emulator *emu);
int is_sign(Emulator *emu);
int is_overflow(Emulator *emu);

/* 減算による EFLAGS の更新関数 */
void update_eflags_sub(Emulator *emu, uint32_t v1, uint32_t v2, uint64_t result);

#endif
