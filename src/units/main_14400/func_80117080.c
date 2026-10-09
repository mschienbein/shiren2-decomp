#include "common.h"
typedef struct { unsigned char pad_0[8]; const void *vtable_8; } Obj;
typedef struct S S;
extern void *func_800AC0C0(S *self, s32 a, s32 b);
extern const unsigned char D_8015DAF8[];
Obj *func_80117080(void *arg, s32 n) { Obj *p=arg; func_800AC0C0((S *)p, 0x12, n); p->vtable_8=D_8015DAF8; return p; }
