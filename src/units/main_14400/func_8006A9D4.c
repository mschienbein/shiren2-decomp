#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

extern s32 D_8013CA20;
extern u8 *D_8016FD64;
extern u8 *D_8016FD6C;

void func_8006A9D4(void *address) {
    u8 *value = address;
    if (D_8013CA20 != 0 && value >= D_8016FD6C) {
        D_8016FD64 = value;
    }
}
