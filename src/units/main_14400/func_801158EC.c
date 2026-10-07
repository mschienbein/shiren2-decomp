#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 delta; s16 index; s32 (*fn)(void *, s32); } VtEntry;
typedef struct { u8 pad[8]; VtEntry *vt; } Obj;
u32 func_8011575C(Obj *o);
s32 func_801158EC(Obj *o) {
    if (func_8011575C(o)) {
        VtEntry *e = &o->vt[3];
        return e->fn((u8 *)o + e->delta, 0x23) ? 1 : 2;
    }
    return 0;
}
