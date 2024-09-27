#include <stdio.h>

#include "constants.h"
#include "decoder.h"
#include "debug.h"
int main(int argc, char *argv[]){
    printf("hello world: %s", argv[0]);
    printf("\nARG:\n");
    for (int i = 1; i < argc; ++i){
        printf("arg %d: %s\n", i, argv[i]);
    }
    PRINTX(argc)
    printf("is 0xE3A00001 alu: %d", IS_ALU_OPERTAION(0xE3A00001));
}