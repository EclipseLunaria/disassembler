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
    flag_t I = (instruction >> 25) & 1;
    uint8_t opcode = (instruction >> 21) & 0xF;
    flag_t S = (instruction >> 20) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    uint16_t operand2 = instruction & 0xFFF;

    char tokens[8][16];
    uint8_t token_count = 0;
    memset(tokens, 0, 8 * 16);

    // select opcode
    strcat(tokens[token_count], ALU_OPCODES[opcode]);
    if (cond != 0xE) {
        strcat(tokens[token_count], COND_TYPE_STRS[cond]);
    }

    if (S) {
        strcat(tokens[token_count], "S");
    }
    strcat(tokens[token_count], " ");

    token_count++;

    // If Instructions are any of the following skip rd
    if (strncmp(tokens[0], "CMP", 3) && strncmp(tokens[0], "CMN", 3) &&
        strncmp(tokens[0], "TST", 3) && strncmp(tokens[0], "TEQ", 3)) {
        // destination reg
        if (rd == 15) {
            strcat(tokens[token_count], "PC");
        } else {
            char dest_reg[8];
            sprintf(dest_reg, "R%d", rd);
            strcat(tokens[token_count], dest_reg);
        }
        token_count++;
    }

    // skip rn if mov or MVN
    if (strncmp(tokens[0], "MOV", 3) && strncmp(tokens[0], "MVN", 3)) {
        char rn_reg[8];
        memset(rn_reg, 0, 8);

        sprintf(rn_reg, "R%d", rn);
        strcat(tokens[token_count], rn_reg);
        token_count++;
    }

    // OP2 decoding

    char shift_buffer[16];
    memset(shift_buffer, 0, 16);
    printf("\nI: %d\n", I);
    if (I) {
        // is immediate op2
        uint32_t imm = operand2 & 0xFF;
        uint32_t rotate = (operand2 >> 8) & 0xF;
        uint32_t result_imm;
        SHFT_ROR(imm, rotate, &result_imm);
        if (result_imm) {
            sprintf(shift_buffer, "#0x%x", result_imm);
        }
        printf("\nFUCK IMM: %d\n", result_imm);
    } else {
        // is register
        reg_t rm = operand2 & 0xF;
        uint8_t shift_type = (operand2 >> 5) & 0b11;

        char rm_reg[8];
        memset(rm_reg, 0, 8);

        sprintf(rm_reg, "R%d", rm);
        strcat(tokens[token_count], rm_reg);
        token_count++;

        if ((operand2 >> 4) & 1) {
            // shift by register
            reg_t rs = (operand2 >> 8) & 0xF;
            sprintf(shift_buffer, "%s R%d", SHIFT_OP_STRS[shift_type], rs);
        } else {
            // shift by immediate
            uint8_t imm_rm_shift = (operand2 >> 7) & 0x1F;
            // skip adding shift token if shift is 0;
            if (imm_rm_shift != 0) {
                sprintf(shift_buffer, "%s #%d", SHIFT_OP_STRS[shift_type], imm_rm_shift);
            }
        }
    }
    strcat(tokens[token_count], shift_buffer);
    if (tokens[token_count][0])
        token_count++;

    for (int i = 0; i < token_count; i++) {
        char join_buf[32];
        memset(join_buf, 0, 16);
        if (i != 0 && i != token_count - 1)
            sprintf(join_buf, "%s, ", tokens[i]);
        else
            sprintf(join_buf, "%s", tokens[i]);

        strcat(buffer, join_buf);
    }
    return 0;
}
