#ifndef DECODER_H
#define DECODER_H
#include <stdint.h>
#include <string.h>

#include "constants.h"

typedef int (*decoder_t)(uint32_t, char*);
typedef uint8_t flag_t;
typedef uint8_t reg_t;

// ARM INSTRUCTIONS
#define IS_MULTIPLY(x) (x & 0x0FC00090) == 0x00000090
#define IS_LONG_MULTIPLY(x) (x & 0x0F800090) == 0x00800090
#define IS_BRANCH_EXCHANGE(x) (x & 0x0FFFFFF0) == 0x012FFF10
#define IS_SWAP(x) (x & 0x0FB00FF0) == 0x012FFF10
#define IS_HALFWORD_REGISTER_TRANSFER(x) (x & 0x0E400FF0) == 0x01000090
#define IS_HALFWORD_IMMEDIATE_TRANSFER(x) (x & 0x0E4000F0) == 0x010000A0
#define IS_SIGNED_DATA_TRANSFER(x) (x & 0x0E0000D0) == 0x000000D0
#define IS_ALU_OPERTAION(x) (x & 0x0C000000) == 0x00000000
#define IS_LOAD_STORE_REGISTER_UBYTE(x) (x & 0x0C000000) == 0x04000000
#define IS_UNDEFINED(x) (x & 0x0E000010) == 0x06000010
#define IS_BLOCK_DATA_TRANSFER(x) (x & 0x0E400000) == 0x08000000
#define IS_BRANCH(x) (x & 0x0E000000) == 0x0A000000
#define IS_SOFTWARE_INTERRUPT(x) (x & 0x0F000000) == 0x0F000000

#define IS_COPROCESSOR_DATA_TRANSFER(x) (x & 0x0F000000) == 0x0F000000
#define IS_COPROCESSOR_DATA_OPERATION(x) (x & 0x0F000000) == 0x0F000000
#define IS_COPROCESSOR_REGISTER_TRANSFER(x) (x & 0x0F000000) == 0x0F000000

int decode_instruction(uint32_t instruction);

decoder_t select_decoder(uint32_t instruction);

int decode_multiply(uint32_t instruction, char* buffer);
int decode_long_multiply(uint32_t instruction, char* buffer);
int decode_branch_exchange(uint32_t instruction, char* buffer);
int decode_swap(uint32_t instruction, char* buffer);
int decode_halfword_register_transfer(uint32_t instruction, char* buffer);
int decode_halfword_immediate_transfer(uint32_t instruction, char* buffer);
int decode_signed_data_transfer(uint32_t instruction, char* buffer);
int decode_alu_operation(uint32_t instruction, char* buffer);
int decode_load_store_data_ubyte(uint32_t instruction, char* buffer);
int decode_undefined(uint32_t instruction, char* buffer);
int decode_block_data_transfer(uint32_t instruction, char* buffer);
int decode_branch(uint32_t instruction, char* buffer);
int decode_software_interrupt(uint32_t instruction, char* buffer);

int decode_coprocessor_data_transfer(uint32_t instruction, char* buffer);
int decode_coprocessor_data_operation(uint32_t instruction, char* buffer);
int decode_coprocessor_register_transfer(uint32_t instruction, char* buffer);

#endif