#include "common.h"

extern s32 D_80139B18;
void func_800556D4(s32 index);
void func_80055BD8(s32 index) {
    if (D_80139B18 != 0) {
        func_800556D4(index);
    }
}
