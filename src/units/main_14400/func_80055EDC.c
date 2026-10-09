#include "common.h"

extern s32 D_80139B30;
extern void func_800556D4(s32 index);

void func_80055EDC(s32 index) {
    if (D_80139B30 == 1) {
        func_800556D4(index);
    }
}
