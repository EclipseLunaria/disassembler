#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "decoder.h"
#include "test_macros.h"

void test_decode_store_writeback_adding_two_registers() {
    // STR R1,[R2,R4]!; Store R1 at R2 and write back R2+R4 to R2.
    char buffer[32];
    memset(buffer, 0, 32);
    uint32_t instruction;
    instruction = 0xE0010392;

    decode_multiply(instruction, buffer);
    printf("buf: %s\n", buffer);

    char* expected = "STR R1, [R2,R4]!";
    CU_ASSERT_EQUAL(strcmp(buffer, expected), 0)
}
void test_decode_store_at_second_register_writeback_sum() {
    // STR R1,[R2],R4; Store R1 at R2 and write back R2+R4 to R2.
    char buffer[32];
    memset(buffer, 0, 32);
    uint32_t instruction;
    instruction = 0xE0010392;

    decode_multiply(instruction, buffer);
    printf("buf: %s\n", buffer);

    char* expected = "STR R1, [R2], R4";
    CU_ASSERT_EQUAL(strcmp(buffer, expected), 0)
}
void test_decode_load_from_reg_with_immediate_offset_no_writeback() {
    // LDR R1,[R2,#16]  ; Load R1 from contents of R2+16, but don’t write back.
    char buffer[32];
    memset(buffer, 0, 32);
    uint32_t instruction;
    instruction = 0xE0010392;

    decode_multiply(instruction, buffer);
    printf("buf: %s\n", buffer);

    char* expected = "LDR R1, [R2, #16]";
    CU_ASSERT_EQUAL(strcmp(buffer, expected), 0)
}
void test_decode_multiply() {
    // LDR R1,[R2,R3,LSL#2]  Load R1 from contents of R2+R3*4.
    char buffer[32];
    memset(buffer, 0, 32);
    uint32_t instruction;
    instruction = 0xE0010392;

    decode_multiply(instruction, buffer);
    printf("buf: %s\n", buffer);

    char* expected = "LDR R1, [R2, R3, LSL #2]";
    CU_ASSERT_EQUAL(strcmp(buffer, expected), 0)
}
void test_decode_multiply() {
    // LDREQBR1,[R6,#5]; Conditionally load byte at R6+5 into R1 bits 0 to 7,
    // filling bits 8 to 31 with zeros.
    char buffer[32];
    memset(buffer, 0, 32);
    uint32_t instruction;
    instruction = 0xE0010392;

    decode_multiply(instruction, buffer);
    printf("buf: %s\n", buffer);

    char* expected = "LDREQBR1, [R6, #5]";
    CU_ASSERT_EQUAL(strcmp(buffer, expected), 0)
}

int add_memory_decoding_tests() {
    CU_pSuite suite = CU_add_suite("Memory Transfer Decoding Tests", 0, 0);

    if (suite == NULL)
        return CU_get_error();

    return CUE_SUCCESS;
}