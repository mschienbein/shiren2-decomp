#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x80]; s16 delta; s16 idx; void (*fn)(void *, s32); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vt; u8 pad28[0x44]; s32 pending; u8 pad70[4]; u8 timer; } Obj;
void func_800E1048(Obj *o) {
    if (--o->timer == 0 && o->pending != 0) {
        o->vt->fn((u8 *)o + o->vt->delta, o->pending);
        o->pending = 0;
    }
}
