#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s16 delta; s16 idx; void (*fn)(void*, s32, void*); } VTE;
typedef struct { u8 pad[0x28]; VTE e28; } VT;
typedef struct { u8 pad[0x18]; VT *vt; } Obj;
extern u8 D_8015DAEC[]; extern void func_800AF174(void*, Obj*); extern void func_800CA4E8(Obj*, void*);
void func_80117180(u8 *a, Obj *b){ func_800AF174(a, b); func_800CA4E8(b, D_8015DAEC); b->vt->e28.fn((u8*)b + b->vt->e28.delta, 4, a+0xC); }
