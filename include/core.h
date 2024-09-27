#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

typedef int (*decoder_t)(uint32_t, char*);
typedef uint8_t flag_t;
typedef uint8_t reg_t;

typedef struct TokenBuilder {
    char tokens[8][16];
    int count;

} TokenBuilder;
#endif