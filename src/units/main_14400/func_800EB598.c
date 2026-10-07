#include "common.h"

typedef unsigned char u8;
typedef short s16;

extern u32 D_8013960C;
s32 func_800EB3A4(void *obj);
s32 func_800A99D0(void);
s32 func_800EB37C(void *obj);
void func_800EB3CC(void *obj, s16 value);
void func_800EB488(void *obj, s16 value);
s32 func_800EB350(void *obj);
void func_800498E4(s32 message_id, ...);

void func_800EB598(void *obj, s16 a1, s16 a2) {
    s32 before = (u8)func_800EB3A4(obj);
    s32 fail = 0;
    s32 diff;

    if (func_800A99D0() != 0 || a1 < 0 || (u8)func_800EB37C(obj) != before) {
        fail = 1;
    }
    if (fail) {
        func_800EB3CC(obj, a1);
        return;
    }
    D_8013960C <<= 1;
    func_800EB488(obj, a2);
    D_8013960C >>= 1;
    func_800EB350(obj);
    diff = (u8)func_800EB3A4(obj) - before;
    if (diff != 0) {
        func_800498E4(0x15, diff);
    }
}
