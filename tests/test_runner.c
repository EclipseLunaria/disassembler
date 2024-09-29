#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

extern int add_macro_tests();
extern int add_selector_tests();
extern int add_alu_decoder_tests();
extern int add_multiply_decoding_tests();
extern int add_memory_decoding_tests();

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    add_macro_tests();
    add_selector_tests();
    add_alu_decoder_tests();
    add_multiply_decoding_tests();
    add_memory_decoding_tests();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}