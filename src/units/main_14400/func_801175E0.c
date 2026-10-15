#include "common.h"
typedef signed short s16;
typedef struct VT {
    char pad[0x90];
    short off;
    s32 (*fn)(void *, s32, s32, unsigned char, s32);
} VT;
typedef struct { char pad[0x24]; VT *vt; } Obj;
/* Damage fields written by func_80136910; natural target extent is 0x14. */
typedef struct {
    void *source;
    u32 kind;
    u32 field_8;
    unsigned short amount;
    unsigned short flags;
    unsigned char field_10;
} Buf;
extern unsigned short D_801569DA;
s32 func_800E1CC4(Obj *o, s32 kind);
void func_80136910(Buf *b, void *source, u32 v, u32 c, u32 d);
void func_800A7ADC(Obj *o, Buf *b);
s32 func_800E1D14(Obj *o, s32 kind);
/* The +0x4C slot supplies the receiver pointer; this override does not use it. */
void func_801175E0(void *unused, void *a, Obj *o) {
    Buf buf;
    Buf *bp;
    s32 mult;
    mult = func_800E1CC4(o, 3) ? 2 : 1;
    bp = &buf;
    func_80136910(bp, a, (u32)(s16)(-D_801569DA * mult), 0x21, 8);
    func_800A7ADC(o, bp);
    if (func_800E1D14(o, 0x13)) o->vt->fn((char *)o + o->vt->off, 1, 0x13, 0, 0);
    if (func_800E1D14(o, 0x14)) o->vt->fn((char *)o + o->vt->off, 1, 0x14, 0, 0);
}
