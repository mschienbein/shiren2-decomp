#include "common.h"
typedef unsigned char u8;
extern u8 D_801F522A;
s32 func_800D1928(void *board, s32 row);
s32 func_800D19C0(void *board, s32 row);
s32 func_800D1A00(void *board, s32 row, s32 col);
s32 func_800D1488(void *self, u8 arg) {
    s32 a = func_800D1928(self, arg);
    s32 i = func_800D19C0(self, arg);
    s32 result;
    if (i < 4) {
        result = -1;
        if (a != 0) result = i;
    } else {
        result = -2;
        if (D_801F522A >= 0xFD) {
            s32 none = 1;
            s32 best = 100;
            s32 bestIdx = -1;
            s32 v;
            i = 0;
            do {
                v = func_800D1A00(self, arg, i);
                if (v != 100) {
                    none = 0;
                    if (v < best) {
                        best = v;
                        bestIdx = i;
                    }
                }
                i++;
            } while (i < 4);
            if (none) {
                result = -3;
            } else if (a == 100 && bestIdx >= 0) {
                result = bestIdx;
            }
        }
    }
    return result;
}
