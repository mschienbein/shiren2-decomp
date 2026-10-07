#include "common.h"

typedef unsigned char u8;

extern s32 D_8013A290;
extern s32 D_801630D0;
extern u8 D_801630D8[];
extern void func_8006EA40(void *arg0);
extern void *func_8006EA64(void *cursor, void *end, void *arg2);
extern void *func_800570C0(void *cursor);
extern void *func_80057974(void *cursor);
extern void *func_800581E8(void *cursor);

void *func_80056FFC(void *arg0) {
    if (D_8013A290 == 0) {
        return arg0;
    }
    func_8006EA40(D_801630D8);
    switch (D_801630D0) {
        case 1:
            arg0 = func_800570C0(arg0);
            break;
        case 2:
            arg0 = func_80057974(arg0);
            break;
        case 3:
            arg0 = func_800581E8(arg0);
            break;
    }
    return func_8006EA64(arg0, 0, D_801630D8);
}
