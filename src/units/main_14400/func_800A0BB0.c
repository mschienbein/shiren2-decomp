#include "common.h"

extern void func_800A0B74(void *stream, unsigned char *output, u32 bit_count);

void func_800A0BB0(void *state, unsigned char *data, s32 count) {
    count--;
    if (count != -1) {
        do {
            func_800A0B74(state, data, 8);
            count--;
            data++;
        } while (count != -1);
    }
}
