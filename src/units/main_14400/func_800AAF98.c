#include "common.h"

typedef unsigned char u8;
extern void *func_800AAD88(u8 a, s32 b);
extern void *func_800AADB4(u8 a, s32 b);

void *func_800AAF98(void) {
    void *object = func_800AAD88(6, 0);
    if (object == 0) {
        object = func_800AADB4(0x84, 0);
    }
    return object;
}
