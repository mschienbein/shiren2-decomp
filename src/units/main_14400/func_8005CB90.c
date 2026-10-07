#include "common.h"

typedef unsigned char u8;

extern u8 D_801DE984[];
extern u8 D_80165974;
s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3);

void func_8005CB90(void) {
    func_8006E908(D_801DE984, 0, 0, 2);
    D_80165974 = 0;
}
