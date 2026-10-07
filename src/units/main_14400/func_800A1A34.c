#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 delta; s16 index; void (*fn)(void *, s32, void *); } VtEntry;
typedef struct { u8 pad[0x18]; VtEntry *vt; } Obj;
void func_800A1A34(void *arg, Obj *o) {
    VtEntry *e = &o->vt[5];
    e->fn((u8 *)o + e->delta, 4, arg);
}
