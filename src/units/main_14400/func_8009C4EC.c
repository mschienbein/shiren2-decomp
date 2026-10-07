#include "common.h"
typedef struct { char data[0x20]; } Elem20;
typedef struct { char pad[0x50]; Elem20 *items; s32 count; } Obj;
void func_80047624(Obj *, Elem20 *, s32, s32);
static inline s32 Obj_count(Obj *o){ return o->count; }
void func_8009C4EC(Obj *o){ s32 i; for (i = 0; i < Obj_count(o); i++) { func_80047624(o, &o->items[i], i, 0); } }
