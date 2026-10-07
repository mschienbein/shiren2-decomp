#include "common.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[0x18]; VEntry *vtbl; } Obj;
extern char D_8014A96C[];
extern char D_80138BF4[];
extern s32 D_80138BF0;
void func_800CA4E8(Obj *, void *);
void func_800462E8(Obj *o) { VEntry *e; func_800CA4E8(o, D_8014A96C); e = &o->vtbl[5]; ((void (*)(char *, s32, void *))e->fn)((char *)o + e->delta, 0x48, D_80138BF4); D_80138BF0 = 0; }
