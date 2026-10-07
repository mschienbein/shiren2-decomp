#include "common.h"
typedef struct { short delta; short index; s32 (*fn)(void *, s32, s32, unsigned char, s32); } VEntry;
typedef struct { char pad[0x24]; VEntry *vtbl; } Obj;
s32 func_800E1CC4(Obj *, s32);
s32 func_80049CB4(s32, ...);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_80118CE0(void *a, Obj *o) { VEntry *e; s32 failed = func_800E1CC4(o, 3) != 1; if (failed) func_80049CB4(0x128, 0x7A); e = &o->vtbl[18]; e->fn((char *)o + e->delta, 0, 3, 0xFE, 0); }
