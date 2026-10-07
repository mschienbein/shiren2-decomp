#include "common.h"

typedef unsigned char u8;

extern const u8 D_8015374C[];
extern const u8 D_80153734[];
void func_800AD3F4(u8 index);
void func_800AD4E0(u8 group) {
    u8 first = D_8015374C[group];
    s32 last = first + D_80153734[group] - 1;
    s32 i;
    for (i = first; i <= last; i++) {
        func_800AD3F4(i);
    }
}
