#include "common.h"
typedef struct { short delta; short idx; void (*fn)(void *, s32, void *); } VEnt;
typedef struct { char pad[0x18]; VEnt *vt; } Obj;
typedef struct { char pad[0x18]; VEnt e3; } VT;
void func_800A1A04(void *arg, Obj *o){ VEnt *e = &((VT*)o->vt)->e3; e->fn((char*)o + e->delta, 4, arg); }
