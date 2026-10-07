#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s16 delta; s16 idx; void *fn; } VTE;
typedef struct { u8 pad[0x90]; VTE e90; VTE e98; } VT;
typedef struct { u8 pad[0x24]; VT *vt; } Obj;
typedef s32 (*Fn5)(void*, s32, s32, u8, s32); typedef void *(*Fn1)(void*);
extern s32 func_800E4454(Obj*); extern void *func_800CF370(void*);
void *func_800E8BD4(Obj *o){ s32 ok = 0; void *v; if (((Fn5)o->vt->e90.fn)((u8*)o + o->vt->e90.delta, 2, 9, 0, 0) == 0) ok = func_800E4454(o) == 0; if (!ok) return 0; v = ((Fn1)o->vt->e98.fn)((u8*)o + o->vt->e98.delta); if (!v) return 0; return func_800CF370(v); }
