#include "constants.h"

char* ALU_OPCODES[] = {"AND", "EOR", "SUB", "RSB", "ADD", "ADC", "SBC", "RSC",
                       "TST", "TEQ", "CMP", "CMN", "ORR", "MOV", "BIC", "MVN"};

char* COND_TYPE_STRS[] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC", "HI", "LS", "GE", "LT", "GT", "LE", "",
};

char* SHIFT_OP_STRS[4] = {"LSL", "LSR", "ASR", "ROR"};