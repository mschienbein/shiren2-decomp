#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s16 delta; s16 pad; void *fn; } VEntry;
typedef struct { VEntry e[9]; } VTable;
typedef struct { s32 unk0; VTable *vt; } Obj;
void func_800CCF80(Obj *o) {
    s32 i;
    ((void (*)(void *, s32))o->vt->e[3].fn)((u8 *)o + o->vt->e[3].delta, 0);
    ((void (*)(void *, s32))o->vt->e[5].fn)((u8 *)o + o->vt->e[5].delta, 0);
    for (i = ((s32 (*)(void *))o->vt->e[1].fn)((u8 *)o + o->vt->e[1].delta) - 1; i >= 0; i--) {
        ((void (*)(void *, s32, void *))o->vt->e[8].fn)((u8 *)o + o->vt->e[8].delta, i, 0);
    }
}
