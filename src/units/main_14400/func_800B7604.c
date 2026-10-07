#include "common.h"
typedef unsigned char u8;
void func_800B7680(void *, s32);
void func_800B7604(void *self, s32 which) {
    u8 i;
    for (i = 0x18; i < 0x1D; i++) {
        if (which == 0) {
            func_800B7680(self, i);
        } else if (which == i) {
            func_800B7680(self, which);
            return;
        }
    }
}
