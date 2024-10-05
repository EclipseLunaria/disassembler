#include "thumb_decoder.h"

int decode_thumb_move_shift_register(uint16_t instruction) {
    uint8_t op = (instruction >> 11) & 0b11;
    uint8_t offset5 = (instruction >> 6) & 0x1F;
    reg_t rs = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_add_subtract(uint16_t instruction) {
    flag_t I = (instruction >> 10) & 1;
    flag_t op = (instruction >> 10) & 1;
    uint8_t rn_offset3 = (instruction >> 6) & 0b111;
    reg_t rs = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;

    return 0;
}

int decode_thumb_immediate_operation(uint16_t instruction) {
    uint8_t op = (instruction >> 11) & 0b11;
    reg_t rd = (instruction >> 8) & 0b111;
    uint8_t offset8 = instruction & 0xFF;
    return 0;
}

int decode_thumb_alu_operation(uint16_t instruction) {
    uint8_t op = (instruction >> 6) & 0xF;
    reg_t rs = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_hi_reg_branch_exchange(uint16_t instruction) {
    uint8_t op = (instruction >> 8) & 0b11;
    flag_t H1 = (instruction >> 7) & 1;
    flag_t H2 = (instruction >> 6) & 1;
    reg_t rs_hs = (instruction >> 3) & 0b111;
    reg_t rd_hd = instruction & 0b111;

    return 0;
}

int decode_thumb_pc_relative_load(uint16_t instruction) {
    reg_t rd = (instruction >> 8) & 0b111;
    uint8_t word8 = instruction & 0xFF;
    return 0;
}

int decode_thumb_transfer_register_offset(uint16_t instruction) {
    flag_t L = (instruction >> 11) & 1;
    flag_t B = (instruction >> 10) & 1;
    reg_t ro = (instruction >> 6) & 0b111;
    reg_t rb = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_transfer_immediate_offset(uint16_t instruction) {
    flag_t B = (instruction >> 12) & 1;
    flag_t L = (instruction >> 11) & 1;
    reg_t offset5 = (instruction >> 6) & 0x1F;
    reg_t rb = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_transfer_signed_byte_halfword(uint16_t instruction) {
    flag_t H = (instruction >> 11) & 1;
    flag_t S = (instruction >> 10) & 1;
    reg_t ro = (instruction >> 6) & 0b111;
    reg_t rb = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_transfer_halfword(uint16_t instruction) {
    flag_t L = (instruction >> 11) & 1;
    reg_t offset5 = (instruction >> 6) & 0x1F;
    reg_t rb = (instruction >> 3) & 0b111;
    reg_t rd = instruction & 0b111;
    return 0;
}

int decode_thumb_sp_relative_transfer(uint16_t instruction) {
    flag_t L = (instruction >> 11) & 1;
    reg_t rd = (instruction >> 8) & 0b111;
    uint8_t word8 = instruction & 0xFF;
    return 0;
}

int decode_thumb_load_address(uint16_t instruction) {
    flag_t SP = (instruction >> 11) & 1;
    reg_t rd = (instruction >> 8) & 0b111;
    uint8_t word8 = instruction & 0xFF;
    return 0;
}

int decode_thumb_add_offset_to_stack_pointer(uint16_t instruction) {
    flag_t S = (instruction >> 7) & 1;
    uint8_t word8 = instruction & 0x7F;
    return 0;
}

int decode_thumb_block_transfer(uint16_t instruction) {
    flag_t L = (instruction >> 11) & 1;
    reg_t rb = (instruction >> 8) & 0b111;
    reg_t rlist = instruction & 0xFF;
    return 0;
}

int decode_thumb_push_pop_registers(uint16_t instruction) {
    flag_t L = (instruction >> 11) & 1;
    flag_t R = (instruction >> 8) & 1;
    reg_t rlist = (instruction >> 6) & 0xF;
    return 0;
    return 0;
    return 0;
}
int decode_thumb_conditional_branch(uint16_t instruction) {
    uint8_t cond = (instruction >> 8) & 0xF;
    uint8_t soffset8 = instruction & 0xFF;
    return 0;
}

int decode_thumb_software_interrupt(uint16_t instruction) {
    uint8_t value8 = instruction & 0xFF;

    return 0;
}
int decode_thumb_unconditional_branch(uint16_t instruction) {
    uint8_t offset11 = instruction & 0xFFF;

    return 0;
}

int decode_thumb_long_branch_with_link(uint16_t instruction) {
    flag_t H = (instruction >> 11) & 1;
    uint8_t offset = instruction & 0x7FF;
    return 0;
}
