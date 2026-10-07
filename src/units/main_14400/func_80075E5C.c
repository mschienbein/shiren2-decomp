#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s16 unk0; s16 h2; u8 pad4[5]; u8 mode; u16 flags; u8 padC[0xA4]; } Entry;
extern Entry D_801DEAB4[];
s32 func_80041FF8(void);
void func_80074BF4(s32, s32, s32);
void func_80075E5C(s32 idx, s32 mode) {
    Entry *e = &D_801DEAB4[idx];
    s32 changed = 0;
    s32 flag = 0;
    if (e->h2 == -1) return;
    if (e->mode == mode) return;
    if (!(mode >= 3 && mode < 8) || !(e->mode >= 3 && e->mode < 8)) changed = 1;
    if (mode >= 3 && mode < 8) {
        if (!(u8)func_80041FF8()) {
            if (!(e->flags & 0x4000)) flag = 1;
        }
    }
    e->mode = mode;
    func_80074BF4(idx, changed, flag);
}
