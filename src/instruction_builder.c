
#include "instruction_builder.h"

int create_token_builder(TokenBuilder* b) {
    memset(b, 0, sizeof(TokenBuilder));
    b->mnemonic = b->tokens[0];
    return 0;
}
int build_instruction(TokenBuilder* b, char* buffer) {
    for (int i = 0; i < b->count; i++) {
        char join_buf[32];
        memset(join_buf, 0, 16);
        if (i != 0 && i != b->count - 1)
            sprintf(join_buf, "%s, ", b->tokens[i]);
        else
            sprintf(join_buf, "%s", b->tokens[i]);

        strcat(buffer, join_buf);
    }
    return 0;
}

int append_token(TokenBuilder* b, char* s) {
    if (b->count >= 8)
        return -1;

    strcpy(b->tokens[b->count], s);
    if (s[0])
        b->count++;
    return 0;
}

int append_register(TokenBuilder* b, reg_t r) {
    if (r > 16 || b->count >= 8)
        return -1;
    build_register_token(r, b->tokens[b->count]);

    b->count++;
    return 0;
}

int append_immediate(TokenBuilder* b, uint32_t value) {
    if (b->count >= 8)
        return -1;

    sprintf(b->tokens[b->count], "#0x%x", value);
    b->count += 1;
    return 0;
}

int build_register_token(reg_t r, char* s) {
    if (r == 15)
        sprintf(s, "%s", "PC");
    else
        sprintf(s, "R%d", r);
    return 0;
}


int build_reg_shift_token(uint16_t shift_bits, char* s) {
    reg_t rm = shift_bits & 0xF;
    uint8_t shift_type = (shift_bits >> 5) & 0b11;
    flag_t reg_shift_flag = (shift_bits >> 4) & 1;
    char reg_token[8];
    memset(reg_token, 0, 8);
    build_register_token(rm, reg_token);

    char shift_buffer[32];
    memset(shift_buffer, 0, 32);

    if (reg_shift_flag) {
        reg_t rs = (shift_bits >> 8) & 0xF;
        char rs_buffer[8];
        memset(rs_buffer, 0, 8);

        build_register_token(rs, rs_buffer);

        sprintf(shift_buffer, ", %s %s", SHIFT_OP_STRS[shift_type], rs_buffer);

    } else {
        // immediate register offset
        uint8_t imm = (shift_bits >> 7) & 0x1F;
        if (imm)
            sprintf(shift_buffer, ", %s #%d", SHIFT_OP_STRS[shift_type], imm);
    }

    // build full shift token
    strcat(s, reg_token);
    strcat(s, shift_buffer);
    return 0;
}
