#ifndef DECODER_THUMB_H
#define DECODER_THUMB_H

#include "core.h"

#define IS_MOVE_SHIFTED_REGISTER(x) (x & 0xE000) == 0 && ((x >> 11) & 0b11) != 0
#define IS_ADD_SUBTRACT(x) (x & 0xF800) == 0x1800
#define IS_IMMEDIATE_OP(x) (x & 0xE) == 0xE000
#define IS_ALU_OPERATION(x) (x & 0xFC00) == 0x4000
#define IS_BX_HI_REG_OP(x) (x & 0xFC00) == 0x4400
#define IS_PC_RELATIVE_LOAD(x) (x & 0xF800) == 0x4800
#define IS_TRANSFER_REGISTER_OFFSET(x) (x & 0xF200) == 0x5000
#define IS_TRANSFER_SIGNED_BYTE_HALFWORD(x) (x & 0xF200) == 0x5200
#define IS_TRANSFER_IMMEDIATE_OFFSET(x) (x & 0xE000) == 0xE000
#define IS_TRANSFER_HALFWORD(x) (x & 0xF000) == 0x8000
#define IS_SP_RELATIVE_TRANSFER(x) (x & 0xF000) == 0x9000
#define IS_LOAD_ADDRESS(x) (x & 0xF000) == 0xA000
#define IS_ADD_STACK_POINTER_OFFSET(x) (x & 0xFF00) == 0xA000
#define IS_PUSH_POP_REGISTERS(x) (x & 0xF600) == 0xA400
#define IS_MULTIPLE_TRANSFER(x) (x & 0xF) == 0xC000
#define IS_CONDITIONAL_BRANCH(x) (x & 0xF) == 0xD000
#define IS_THUMB_SOFTWARE_INTERRUPT(x) (x & 0xFF) == 0xDF00
#define IS_UNCONDITIONAL_BRANCH(x) (x & 0xF800) == 0xE000
#define IS_LONG_BRANCH_WITH_LINK(x) (x & 0xF000) == 0xF000

int decode_thumb_instruction(uint16_t instruction) {

    return 0;
}

#endif