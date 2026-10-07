#include "common.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[0x18]; VEntry *vtbl; } Obj;
typedef struct { char pad[0xC]; char unkC[1]; } Arg;
extern char D_8015D9CC[];
void func_800AF174(Arg *, Obj *);
void func_800CA4E8(Obj *, void *);
void func_8011640C(Arg *a, Obj *o) { VEntry *e; func_800AF174(a, o); func_800CA4E8(o, D_8015D9CC); e = &o->vtbl[5]; ((void (*)(char *, s32, void *))e->fn)((char *)o + e->delta, 1, a->unkC); }
