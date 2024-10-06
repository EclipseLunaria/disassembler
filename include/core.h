#ifndef CORE_H
#define CORE_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

typedef int (*decoder_t)(uint32_t, char*);
typedef uint8_t flag_t;
typedef uint8_t reg_t;

typedef struct OpFlags {
    uint8_t I : 1;
    uint8_t P : 1;
    uint8_t U : 1;
    uint8_t B : 1;
    uint8_t L : 1;
    uint8_t W : 1;
    uint8_t H : 1;
    uint8_t S : 1;
} OpFlags;

typedef struct TokenBuilder {
    char tokens[8][16];
    char _buffer[BUFFER_SIZE];
    int count;
    char* mnemonic;

} TokenBuilder;
#endif