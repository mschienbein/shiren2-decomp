#include "common.h"

typedef unsigned char u8;

extern u8 D_80160B00[];
extern u8 D_00194FC0[];
extern u8 D_2000948[];
void func_8006AC30(void *, void *, void *, s32, s32, s32);
/* receiver: unused; callers (0x80109EE8, 0x80109F30) pass the object pointer in a0. */
u8 *func_80044D74(void *receiver, u8 index) {
    func_8006AC30(D_80160B00, D_00194FC0, D_2000948, 8, index - 1, 1);
    return D_80160B00;
}
