#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s16 delta; s16 pad; s32 (*fn)(void *, s32, s32, u8, s32); } VEntry;
typedef struct { VEntry e[19]; } VTable;
typedef struct { u8 pad[0x24]; VTable *vt; } S;
extern u16 D_801569DA;
extern u16 D_801569DC;
s32 func_800E1CC4(S *, s32);
s16 func_800E0690(S *, s16, s16);
s32 func_80049CB4(s32, ...);
s32 func_800E1D14(S *, s32);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_801174F0(void *unused, S *s) {
    s16 mul = func_800E1CC4(s, 3) ? 2 : 1;
    if (func_800E0690(s, D_801569DA * mul, D_801569DC * mul) > 0) func_80049CB4(0x128, 0x6D);
    if (func_800E1D14(s, 0x13)) s->vt->e[18].fn((u8 *)s + s->vt->e[18].delta, 1, 0x13, 0, 0);
    if (func_800E1D14(s, 0x14)) s->vt->e[18].fn((u8 *)s + s->vt->e[18].delta, 1, 0x14, 0, 0);
}
