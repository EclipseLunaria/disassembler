#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "instruction_builder.h"
#include "test_macros.h"

void test_build_immediate_shift_token() {
    // build shift bits
    uint8_t shift_type = 0b01;
    uint16_t imm_value = 4;
    reg_t rm = 5;

    uint16_t shift_bits = imm_value << 7 | shift_type << 5 | rm;
    char actual[16];
    memset(actual, 0, 16);
    build_reg_shift_token(shift_bits, actual);
    printf("shift buffer: %s\n", actual);
    CU_ASSERT_STRING_EQUAL(actual, "R5, LSR #4")
}
void test_build_immediate_shift_token_shift_value_zero() { // build shift bits
    uint8_t shift_type = 0b01;
    uint16_t imm_value = 0;
    reg_t rm = 5;

    uint16_t shift_bits = imm_value << 7 | shift_type << 5 | rm;
    char actual[16];
    memset(actual, 0, 16);
    build_reg_shift_token(shift_bits, actual);
    printf("shift buffer: %s\n", actual);
    CU_ASSERT_STRING_EQUAL(actual, "R5")
}
void test_build_register_shift_token() {
    uint8_t shift_type = 0b11;
    uint16_t rs = 2;
    uint16_t rm = 5;

    uint16_t shift_bits = rs << 8 | shift_type << 5 | 1 << 4 | rm;
    char actual[16];
    memset(actual, 0, 16);
    build_reg_shift_token(shift_bits, actual);
    printf("shift buffer: %s\n", actual);
    CU_ASSERT_STRING_EQUAL(actual, "R5, ROR R2")
}

int add_instruction_builder_tests() {
    CU_pSuite suite = CU_add_suite("Instruction Builder Tests", 0, 0);

    if (suite == NULL)
        return CU_get_error();

    ADD_TEST(test_build_immediate_shift_token);
    ADD_TEST(test_build_immediate_shift_token_shift_value_zero);
    ADD_TEST(test_build_register_shift_token);

    return CUE_SUCCESS;
}