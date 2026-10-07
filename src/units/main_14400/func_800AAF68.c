#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void *func_800AAD88(u8, s32);
void *func_800AADB4(u8, s32);
void *func_800AAF68(void) {
    void *object = func_800AAD88(4, 0);
    if (object == 0) {
        object = func_800AADB4(0x5B, 0);
    }
    return object;
}
