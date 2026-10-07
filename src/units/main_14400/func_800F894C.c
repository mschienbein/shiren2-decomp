#include "common.h"

typedef unsigned char u8;
void *func_800A38FC(s32);
void *func_800F8900(void *, u8);
void *func_800F894C(u8 arg0) {
    return func_800F8900(func_800A38FC(0x84), arg0);
}
