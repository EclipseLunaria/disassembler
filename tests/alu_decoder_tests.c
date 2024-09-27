#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "decoder.h"
#include "test_macros.h"

void test_decode_alu_mnemonic() {
    uint32_t instruction;
    // MOV R0, #1
    instruction = 0xE3A00001;
    char buffer1[32];
    memset(buffer1, 0, 32);
    decode_alu_operation(instruction, buffer1);
    char* expected = "MOV";
    CU_ASSERT_EQUAL(strncmp(buffer1, expected, 3), 0)
}

void test_decode_mov_with_cond() {
    uint32_t instruction;
    // MOV R0, #1
    instruction = 0x03A00001;
    char buffer1[32];
    memset(buffer1, 0, 32);
    decode_alu_operation(instruction, buffer1);
    char* expected = "MOVEQ";
    CU_ASSERT_EQUAL(strncmp(buffer1, expected, 5), 0)
}

void test_decode_mov_with_s_bit() {
    uint32_t instruction;
    // MOV R0, #1
    instruction = 0xE3B00001;
    char buffer1[32];
    memset(buffer1, 0, 32);
    decode_alu_operation(instruction, buffer1);
    printf("buf: %s\n", buffer1);
    char* expected = "MOVS";
    CU_ASSERT_EQUAL(strncmp(buffer1, expected, 5), 0)
}
void test_decode_mov_with_s_bit_and_cond() {
    uint32_t instruction;
    // MOV R0, #1
    instruction = 0x23B00001;
    char buffer1[32];
    memset(buffer1, 0, 32);
    decode_alu_operation(instruction, buffer1);
    printf("buf: %s\n", buffer1);
    char* expected = "MOVCSS";
    CU_ASSERT_EQUAL(strncmp(buffer1, expected, 5), 0)
}

int add_alu_decoder_tests() {
    CU_pSuite suite = CU_add_suite("ALU Decoder Tests", 0, 0);

    if (suite == NULL)
        return CU_get_error();

    ADD_TEST(test_decode_alu_mnemonic)
    ADD_TEST(test_decode_mov_with_cond)
    ADD_TEST(test_decode_mov_with_s_bit)
    ADD_TEST(test_decode_mov_with_s_bit_and_cond)
    return CUE_SUCCESS;
}