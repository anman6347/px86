#ifndef EMULATOR_FUNCTION_H_
#define EMULATOR_FUNCTION_H_

#include <stdint.h>

#include "emulator.h"

/* memory 配列の eip が指す番地から 8bit の符号無し整数値を取得し、32bit
で返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code8(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 8bit の符号付き整数値を取得し、32bit
で返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code8(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 32bit の符号無し整数値を取得し、32bit
 * で返す。第 2 引数で eip からのオフセットを指定できる */
uint32_t get_code32(Emulator *emu, int index);

/* memory 配列の eip が指す番地から 32bit の符号付き整数値を取得し、32bit
で返す。第 2 引数で eip からのオフセットを指定できる */
int32_t get_sign_code32(Emulator *emu, int index);

/* index 番目の汎用レジスタに 32bit 値を取得する */
uint32_t get_register32(Emulator* emu, int index);

/* index 番目の汎用レジスタに 32bit 値を設定する */
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
void push32(Emulator* emu, uint32_t value);

/* スタックから32bit値を取りだす */
uint32_t pop32(Emulator* emu);

#endif
