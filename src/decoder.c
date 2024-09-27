#include "decoder.h"

decoder_t select_decoder(uint32_t instruction) {
    if (IS_ALU_OPERTAION(instruction)) {
        return decode_alu_operation;
    }

    return 0;
}

int decode_multiply(uint32_t instruction, char* buffer) {
    return 0;
}
int decode_long_multiply(uint32_t instruction, char* buffer) {
    return 0;
}
int decode_alu_operation(uint32_t instruction, char* buffer) {
    uint8_t cond = (instruction >> 28) & 0xFF;
    // flag_t I = (instruction >> 25) & 1;
    uint8_t opcode = (instruction >> 21) & 0xF;
    flag_t S = (instruction >> 20) & 1;
    // reg_t rn = (instruction >> 16) & 0xF;
    // reg_t rd = (instruction >> 12) & 0xF;
    // uint16_t operand2 = instruction & 0xFFF;

    // select opcode
    strcat(buffer, ALU_OPCODES[opcode]);
    if (cond != 0xE) {
        strcat(buffer, COND_TYPE_STRS[cond]);
    }

    if (S) {
        strcat(buffer, "S");
    }
    return 0;
}
