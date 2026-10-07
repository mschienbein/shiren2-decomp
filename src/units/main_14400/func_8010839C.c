#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s16 delta; s16 pad; s32 (*fn)(void *); } VEntry;
typedef struct { u8 pad[0x20]; VEntry e20; } VTable;
typedef struct { s32 unk0; VTable *vt; } Obj;
typedef struct { u8 pad[0x8C]; Obj *obj; u8 pad2[0x2C]; s32 xBC; s32 xC0; } S;
s32 func_80107FA0(S *);
s32 func_801081A4(S *);
s32 func_8010839C(S *s) {
    if (s->xBC != 0) {
        if (s->xC0 != 0) {
            return func_80107FA0(s);
        }
    } else {
        Obj *o = s->obj;
        if (!o->vt->e20.fn((u8 *)o + o->vt->e20.delta)) {
            return func_80107FA0(s);
        }
    }
    return func_801081A4(s);
}
