#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x68]; s16 delta; s16 idx; s32 (*fn)(void *, s32); } VTable;
typedef struct { u8 pad0[0x20]; s32 index; u8 pad24[0x28]; VTable *vt; } Obj;
typedef struct { s32 base; s32 stride; } Span;
s32 func_8009829C(Obj *o, Span *s) {
    return o->vt->fn((u8 *)o + o->vt->delta, s->base + o->index * s->stride);
}
