#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 delta; s16 index; s32 (*fn)(void *, s32); } VtEntry;
typedef struct { u8 pad[8]; VtEntry *vt; u16 xC; } Obj;
u16 func_800AE710(Obj *o) {
    VtEntry *e = &o->vt[3];
    if (e->fn((u8 *)o + e->delta, 0x1E)) {
        return o->xC;
    }
    return 0;
}
