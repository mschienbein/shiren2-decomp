#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x48]; s16 delta; s16 idx; s32 (*fn)(void *); } VTable;
typedef struct { u8 pad[0xC]; VTable *vt; s32 active; } Obj;
s32 func_80092084(Obj *o) {
    if (o->active == 0) return -1;
    return o->vt->fn((u8 *)o + o->vt->delta);
}
