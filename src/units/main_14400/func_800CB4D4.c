#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s16 delta; s16 pad; void (*fn)(void *, s32); } VEntry;
typedef struct { u8 pad[0x10]; VEntry e10; } VTable;
typedef struct { u8 pad[0x18]; VTable *vt; } Obj;
typedef struct { s32 unk0; Obj *obj; } S;
void func_800CB4D4(S *s) { Obj *o = s->obj; if (o) o->vt->e10.fn((u8 *)o + o->vt->e10.delta, 3); }
