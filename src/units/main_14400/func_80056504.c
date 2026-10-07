#include "common.h"

extern s32 D_8013A260;
void func_800556D4(s32 index);
void func_80056504(s32 index) {
    if (D_8013A260 != 0) {
        func_800556D4(index);
    }
}
