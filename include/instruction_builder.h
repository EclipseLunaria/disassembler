#ifndef TOKEN_BUILDER_H
#define TOKEN_BUILDER_H

#include "core.h"

int create_token_builder(TokenBuilder* b);

int build_register_token(reg_t r, char* s);
int build_reg_shift_token(uint16_t shift_bits, char* s);

int append_token(TokenBuilder* b, char* s);
int append_register(TokenBuilder* b, reg_t r);
int append_immediate(TokenBuilder* b, uint32_t value);

int build_instruction(TokenBuilder* b, char* buffer);
#endif