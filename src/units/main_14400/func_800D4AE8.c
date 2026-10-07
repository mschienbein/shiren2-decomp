#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s16 delta; s16 index; void (*fn)(void *self, s32 flags); } VEntry;
typedef struct { u8 pad[8]; VEntry *vtbl; } Obj;
typedef struct { void *p; } S;
Obj *func_800AFD78(void *, u8);
s32 func_800AF920(void *, u8);
void func_800D4AE8(S *s, s32 key) {
    u8 b = key;
    Obj *o;
    VEntry *e;
    if (s->p != 0 && (func_800AF920(s->p, b) ^ 1) == 0) {
        o = func_800AFD78(s->p, b);
        if (o != 0) {
            e = &o->vtbl[1];
            e->fn((u8 *)o + e->delta, 3);
        }
    }
}
