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
    if (IS_LOAD_STORE_REGISTER_UBYTE(instruction)) {
        return decode_load_store_data_ubyte;
    }
    if (IS_UNDEFINED(instruction)) {
        return decode_undefined;
    }
    if (IS_BLOCK_DATA_TRANSFER(instruction)) {
        return decode_block_data_transfer;
    }
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
    uint8_t cond = (instruction >> 28) & 0xF;
    uint8_t L = (instruction >> 28) & 0xFF;
    uint32_t shift = instruction & 0xFFFF;

    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, "B");
    strcat(mnemonic_buffer, L ? "L" : "");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
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
    // extract parameters
    uint8_t cond = (instruction >> 28) & 0xF;
    flag_t I = (instruction >> 25) & 1;
    flag_t P = (instruction >> 24) & 1;
    flag_t U = (instruction >> 23) & 1;
    flag_t B = (instruction >> 22) & 1;
    flag_t W = (instruction >> 21) & 1;
    flag_t L = (instruction >> 20) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    uint32_t offset = instruction & 0xFFF;

    TokenBuilder builder;
    create_token_builder(&builder);
    // build mnemonic
    char mnemonic_buffer[16];
    memset(mnemonic_buffer, 0, 16);
    strcat(mnemonic_buffer, L ? "LDR" : "STR");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, B ? "B" : "");
    strcat(mnemonic_buffer, W && !P ? "T" : "");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rd);

    // build memory token
    char address_buffer[64];
    memset(address_buffer, 0, 64);

    strcat(address_buffer, "[");

    // add rn register
    char rn_buffer[16];
    memset(rn_buffer, 0, 16);
    build_register_token(rn, rn_buffer);
    strcat(address_buffer, rn_buffer);

    // build offset buffer
    char offset_buffer[32];
    memset(offset_buffer, 0, 32);
    if (I) {
        if (offset) {
            sprintf(offset_buffer, "#%s0x%x", U ? "" : "-", offset);
        }
    } else {
        build_reg_shift_token(offset, offset_buffer);
        if (P) {

            strcat(address_buffer, ", ");
            strcat(address_buffer, U ? "" : "-");
            printf("\nOFFSET BUFFER: %s\n", offset_buffer);
            strcat(address_buffer, offset_buffer);
        }
    }

    strcat(address_buffer, "]");
    strcat(address_buffer, offset && W && P ? "!" : "");
    append_token(&builder, address_buffer);

    if (!P)
        append_token(&builder, offset_buffer);

    build_instruction(&builder, buffer);
    return 0;
}

int decode_block_data_transfer(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);

    uint8_t cond = (instruction >> 28) & 0xF;
    flag_t P = (instruction >> 24) & 1;
    flag_t U = (instruction >> 23) & 1;
    flag_t S = (instruction >> 22) & 1;
    flag_t W = (instruction >> 21) & 1;
    flag_t L = (instruction >> 20) & 1;
    uint8_t transfer_type = L << 2 | P << 1 | U;
    reg_t rn = (instruction >> 16) & 0xF;
    uint16_t reg_list = instruction & 0xFFFF;
    char mnemonic_buffer[1024];
    memset(mnemonic_buffer, 0, 1024);
    strcat(mnemonic_buffer,
           rn == 14 ? STACK_BLOCK_SUFFIXS[transfer_type] : OTHER_BLOCK_SUFFIXS[transfer_type]);
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    // possibly break this down into a method
    char reg_token[1024];
    memset(reg_token, 0, 1024);
    build_register_token(rn, reg_token);
    strcat(reg_token, W ? "!" : "");
    append_token(&builder, reg_token);
    // build register list
    char reg_list_buffer[1024];
    memset(reg_list_buffer, 0, 1024);
    build_register_list(reg_list, reg_list_buffer);
    strcat(reg_list_buffer, S ? "^" : "");
    append_token(&builder, reg_list_buffer);

    build_instruction(&builder, buffer);
    return 0;
}

int decode_halfword_register_transfer(uint32_t instruction, char* buffer) {
    TokenBuilder builder;
    create_token_builder(&builder);
    uint8_t cond = instruction >> 28;
    flag_t P = (instruction >> 24) & 1;
    flag_t U = (instruction >> 23) & 1;
    flag_t I = (instruction >> 22) & 1;
    flag_t W = (instruction >> 21) & 1;
    flag_t L = (instruction >> 20) & 1;
    reg_t rn = (instruction >> 16) & 0xF;
    reg_t rd = (instruction >> 12) & 0xF;
    flag_t S = (instruction >> 6) & 1;
    flag_t H = (instruction >> 5) & 1;
    assert((S << 1 | H) & 0b11 != 0);
    char mnemonic_buffer[1024];
    memset(mnemonic_buffer, 0, 1024);
    strcat(mnemonic_buffer, L ? "LDR" : "STR");
    strcat(mnemonic_buffer, COND_TYPE_STRS[cond]);
    strcat(mnemonic_buffer, S ? "S" : "");
    strcat(mnemonic_buffer, H ? "H" : "B");
    strcat(mnemonic_buffer, " ");
    append_token(&builder, mnemonic_buffer);

    append_register(&builder, rd);

    char rn_buffer[1024];
    memset(rn_buffer, 0, 1024);

    char address_buffer[1024];
    memset(address_buffer, 0, 1024);
    strcat(address_buffer, rn_buffer);

    // build offset buffer
    char offset_buffer[32];
    memset(offset_buffer, 0, 32);
    if (I) {
        if (offset) {
            sprintf(offset_buffer, "#%s0x%x", U ? "" : "-", offset);
        }
    } else {
        build_reg_shift_token(offset, offset_buffer);
        if (P) {

            strcat(address_buffer, ", ");
            strcat(address_buffer, U ? "" : "-");
            printf("\nOFFSET BUFFER: %s\n", offset_buffer);
            strcat(address_buffer, offset_buffer);
        }
    }

    strcat(address_buffer, "]");
    strcat(address_buffer, offset && W && P ? "!" : "");
    append_token(&builder, address_buffer);

    if (!P)
        append_token(&builder, offset_buffer);
}

int decode_software_interrupt(uint32_t instruction, char* buffer) {
    strcat(buffer, "SWI");
    return 0;
}

int decode_undefined(uint32_t instruction, char* buffer) {
    strcat(buffer, "<Undefined>");
    return 0;
}
