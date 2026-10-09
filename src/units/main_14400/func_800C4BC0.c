#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 0x14-byte event: base part (owner pointer, short, count) from func_800C4840, vtable at
 * +0x0C and the target pointer at +0x10. */
typedef struct {
    u8 pad0[0xC];
    void *vtable;
    void *x10;
} S;

extern u8 D_80153FC8[];

S *func_800C4840(S *obj, void *owner, s32 value);

/* Constructor: base init with (owner, value), then this class's vtable and field 0x10 = target. */
S *func_800C4BC0(S *s, void *owner, void *target, u16 c)
{
    func_800C4840(s, owner, c);
    s->vtable = D_80153FC8;
    s->x10 = target;
    return s;
}
