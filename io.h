#ifndef IO_H_
#define IO_H_

#include <stdint.h>

/* in のオペランドに指定された番地が 0x03f8 なら文字の入力を行う */
uint8_t io_in8(uint16_t address);

/* out のオペランドに指定された番地が　0x03f8 なら文字の出力を行う */
void io_out8(uint16_t address, uint8_t value);

#endif
