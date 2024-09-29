#include "decoder.h"

int decode_instruction(uint32_t instruction, char* buffer) {
    decoder_t decoder = select_decoder(instruction);

    if (((instruction >> 28) & 0xF) == 0xF || !decoder) {
        strcpy(buffer, "UNKNOWN");
        return -1;
    }

    decoder(instruction, buffer);
    return 0;
}

decoder_t select_decoder(uint32_t instruction) {
    if (IS_MULTIPLY(instruction)) {
        return decode_multiply;
    }
    if (IS_LONG_MULTIPLY(instruction)) {
        return decode_long_multiply;
    }
    if (IS_BRANCH_EXCHANGE(instruction)) {
        return decode_branch_exchange;
    }
    if (IS_SWAP(instruction)) {
        return decode_swap;
    }
    // if (IS_HALFWORD_REGISTER_TRANSFER(instruction)) {
    // }
    // if (IS_HALFWORD_IMMEDIATE_TRANSFER(instruction)) {
    // }
    // if (IS_SIGNED_DATA_TRANSFER(instruction)) {
    // }
    if (IS_ALU_OPERTAION(instruction)) {
        return decode_alu_operation;
    }
    // if (IS_LOAD_STORE_REGISTER_UBYTE(instruction)) {
    // }
    if (IS_UNDEFINED(instruction)) {
        return decode_undefined;
    }
    // if (IS_BLOCK_DATA_TRANSFER(instruction)) {
    // }
    if (IS_BRANCH(instruction)) {
        return decode_branch;
    }
    if (IS_SOFTWARE_INTERRUPT(instruction)) {
        return decode_software_interrupt;
    }

    return NULL;
}

int decode_multiply(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xFF;
    flag_t A = (instruction >> 21) & 1;
    flag_t S = (instruction >> 20) & 1;
    reg_t rd = (instruction >> 16) & 0xF;
    reg_t rn = (instruction >> 12) & 0xF;
    reg_t rs = (instruction >> 8) & 0xF;
    reg_t rm = instruction & 0xF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, A ? "MLA" : "MUL");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, S ? "S" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rd);
    append_register(&builder, rm);
    append_register(&builder, rs);
    if (A)
        append_register(&builder, rn);

    build_instruction(&builder, buffer);
    return 0;
}
int decode_long_multiply(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xFF;
    flag_t U = (instruction >> 20) & 1;
    flag_t A = (instruction >> 21) & 1;
    flag_t S = (instruction >> 20) & 1;
    reg_t rdhi = (instruction >> 16) & 0xF;
    reg_t rdlo = (instruction >> 12) & 0xF;
    reg_t rs = (instruction >> 8) & 0xF;
    reg_t rm = instruction & 0xF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);

    strcat(mnemonic_buffer, U ? "S" : "U");
    strcat(mnemonic_buffer, A ? "MLA" : "MUL");
    strcat(mnemonic_buffer, "L");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, S ? "S" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rdlo);
    append_register(&builder, rdhi);
    append_register(&builder, rm);
    append_register(&builder, rs);

    build_instruction(&builder, buffer);
    return 0;
}
#define IS_REGISTER_SHIFTED(x)
int decode_alu_operation(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xFF;
    flag_t I = (instruction >> 25) & 1;
    uint8_t opcode = (instruction >> 21) & 0xF;
    flag_t S = (instruction >> 20) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    uint16_t operand2 = instruction & 0xFFF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, ALU_OPCODES[opcode]);
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, S ? "S" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    if (strncmp(builder.mnemonic, "CMP", 3) && strncmp(builder.mnemonic, "CMN", 3) &&
        strncmp(builder.mnemonic, "TST", 3) && strncmp(builder.mnemonic, "TEQ", 3)) {
        append_register(&builder, rd);
    }

    if (strncmp(builder.mnemonic, "MOV", 3) && strncmp(builder.mnemonic, "MVN", 3)) {
        append_register(&builder, rn);
    }

    if (I) {
        uint32_t imm = operand2 & 0xFF;
        uint32_t rotate = (operand2 >> 8) & 0xF;
        uint32_t result_imm;
        SHFT_ROR(imm, rotate, &result_imm);
        if (result_imm) {
            append_immediate(&builder, result_imm);
        }
    } else {
        reg_t rm = operand2 & 0xF;
        flag_t is_register_shifted = (operand2 >> 4) & 1;
        uint8_t shift_type = (operand2 >> 5) & 0b11;
        append_register(&builder, rm);
        if (is_register_shifted) {
            // shift by register
            reg_t rs = (operand2 >> 8) & 0xF;
            append_register(&builder, rs);
        } else {
            // shift by immediate
            char shift_buffer[16];
            memset(shift_buffer, 0, 16);
            uint8_t imm_rm_shift = (operand2 >> 7) & 0x1F;
            if (imm_rm_shift != 0) {
                sprintf(shift_buffer, "%s #%d", SHIFT_OP_STRS[shift_type], imm_rm_shift);
            }
            append_token(&builder, shift_buffer);
        }
    }

    build_instruction(&builder, buffer);

    return 0;
}

// Needs Testing

int decode_branch_exchange(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xFF;
    reg_t rn = instruction & 0xF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, "BX");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rn);
    build_instruction(&builder, buffer);
    return 0;
}

int decode_branch(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t L = (instruction >> 28) & 0xFF;
    uint32_t shift = instruction & 0xFFFF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, "B");
    strcat(mnemonic_buffer, L ? "L" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    // needs to test sign maybe needs extenstion
    append_immediate(&builder, shift);
    build_instruction(&builder, buffer);
    return 0;
}

int decode_swap(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xFF;
    flag_t B = (instruction >> 22) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    reg_t rm = instruction & 0xF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, "SWP");
    strcat(mnemonic_buffer, B ? "B" : "");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rd);
    append_register(&builder, rm);

    // possibly break this down into a method
    char b[16];
    memset(b, 0, 16);
    sprintf(b, "[%d]", rn);
    append_token(&builder, b);

    build_instruction(&builder, buffer);
    return 0;
}

// memory instructions

int decode_load_store_data_ubyte(uint32_t instruction, char* buffer) {
    uint8_t cond = (instruction >> 28) & 0xF;
    flag_t I = (instruction >> 25) & 1;
    flag_t P = (instruction >> 24) & 1;
    // flag_t U = (instruction >> 23) & 1;
    flag_t B = (instruction >> 22) & 1;
    flag_t W = (instruction >> 21) & 1;
    flag_t L = (instruction >> 20) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    uint32_t offset = instruction & 0xFFF;

    TokenBuilder builder;
    create_token_builder(&builder);

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, L ? "LDR" : "STR");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, B ? "B" : "");
    strcat(mnemonic_buffer, W && P ? "T" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rd);

    // build memory token
    char address_buffer[64];
    memset(address_buffer, 0, 64);
    if (P) {
        sprintf(address_buffer, "[R%d", rn);

        if (I) {
            if (offset) {
                // <expression>
            }
        } else {
            reg_t rm = offset & 0xF;
            uint32_t imm = (offset >> 4) & 0xFF;
            //handle shift op
            
            strcat(address_buffer, ", R");
        }
        strcat(address_buffer, "]");
        strcat(address_buffer, offset && W ? "!" : "");
    }
    build_instruction(&builder, buffer);
    return 0;
}

int decode_software_interrupt(uint32_t instruction, char* buffer) {
    strcat(buffer, "SWI");
    return 0;
}

int decode_undefined(uint32_t instruction, char* buffer) {
    strcat(buffer, "<Undefined>");
    return 0;
}
