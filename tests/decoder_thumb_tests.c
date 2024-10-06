#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "test_macros.h"
#include "thumb_decoder.h"

int add_thumb_instruction_tests() {
    CU_pSuite suite = CU_add_suite("Thumb Decoder Tests", 0, 0);

    if (suite == NULL)
        return CU_get_error();

    // test alu op macros

    return CUE_SUCCESS;
}