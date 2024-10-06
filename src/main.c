#include <stdio.h>

#include "arm_decoder.h"
#include "constants.h"
#include "debug.h"
int main(int argc, char* argv[]) {
    uint32_t READ_START = 0;
    size_t limit_write = 0xFFFFFFF;
    char* outfile = "dump";
    printf("argc %d\n", argc);
    if (argc == 1) {
        printf("NO FILE SPECIFIED\n");
        return -1;
    }

    FILE* rom = fopen(argv[1], "r");
    FILE* out = fopen(outfile, "w");

    if (rom == NULL) {
        perror("Error opening file");
        return 1;
    }
    if (out == NULL) {
        perror("Error opening file");
        return 1;
    }

    uint32_t address = READ_START;
    uint32_t buffer[1024];
    memset(buffer, 0, 1024 * 4);
    int sz = fread(buffer, sizeof(uint32_t), 1024, rom);
    printf("\nsz: %d", sz);
    while (sz > 0) {

        for (int i = 0; i < sz; ++i) {
            char ibuf[128];
            memset(ibuf, 0, 128);

            decode_arm_instruction(buffer[i], ibuf);
            char outline[256];
            memset(outline, 0, 256);

            if (buffer[i] != 0xFFFFFFFF)
                fprintf(out, "%06x:\t\t%08x\t%s\n", address, buffer[i], ibuf);
            address++;
            if (address >= limit_write) {
                fflush(out);
                fclose(rom);
                fclose(out);
                return 0;
            }
        }
        fflush(out);
        fread(buffer, sizeof(uint32_t), 1024, rom);
    }
    fflush(out);
    fclose(rom);
    fclose(out);
}