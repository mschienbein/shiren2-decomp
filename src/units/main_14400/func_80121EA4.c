#include "common.h"

typedef unsigned char u8;
typedef short s16;
/* D_80154550 slot 4 (+0x20/+0x24) targets func_800CE710: s32 count(self). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self);
} VTableEntry;
typedef struct {
    char pad0[4];
    VTableEntry *vtable;
} Base;
typedef struct {
    char pad0[0xC];
    Base base;
    char pad14[0x14];
    u8 field_28;
    char pad29[3];
    u8 field_2C;
} S;
s32 func_80121EA4(S *s, s32 kind) {
    if (kind == 0x1D) {
        return s->field_28 != 0;
    }
    if (kind == 0x1B) {
        Base *b = &s->base;
        return b->vtable[4].func((char *)b + b->vtable[4].delta) != 0;
    }
    if (kind == 0x1C) {
        s32 r = 0;
        if (s->field_2C != 0xFF) {
            Base *b = &s->base;
            r = b->vtable[4].func((char *)b + b->vtable[4].delta) == 0;
        }
        return r;
    }
    return kind == 0x11 || kind == 0x13 || kind == 0xB;
}
