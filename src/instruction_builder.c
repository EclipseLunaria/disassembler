
#include "instruction_builder.h"

int create_token_builder(TokenBuilder* b) {
    memset(b, 0, sizeof(TokenBuilder));
    b->mnemonic = b->tokens[0];
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

    if (r == 15)
        sprintf(b->tokens[b->count], "%s", "PC");
    else
        sprintf(b->tokens[b->count], "R%d", r);

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
