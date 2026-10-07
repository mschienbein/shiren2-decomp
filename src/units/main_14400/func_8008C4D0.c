#include "common.h"
typedef unsigned char u8;
typedef struct { u8 unk0[2]; u8 flags; u8 unk3[0x75]; } Entry;
extern s32 D_8013FEE0;
extern u8 *D_8013FEE4;
void func_8008C4D0(s32 index, u8 enable) {
    if (D_8013FEE0 != 0) {
        Entry *e = &((Entry *)(D_8013FEE4 + 0x100))[index];
        if (enable) {
            e->flags |= 2;
        } else {
            e->flags &= ~2;
        }
    }
}
