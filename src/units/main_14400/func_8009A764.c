#include "common.h"
typedef unsigned short u16;
typedef struct { short delta; short index; void (*fn)(void *); } VEntry;
typedef struct { char pad[0x4C]; VEntry *vtbl; } Obj;
void func_80097460(Obj *, s32);
void func_8009A764(Obj *self, s32 mode) {
    VEntry *e;
    func_80097460(self, mode);
    e = &self->vtbl[2];
    e->fn((char *)self + e->delta);
}
