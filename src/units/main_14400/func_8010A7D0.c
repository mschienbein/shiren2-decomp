#include "common.h"
typedef unsigned char u8;
extern char D_80159130[], D_8015C9F0[];
extern void *func_800EE3C0(void *, s32, u8);
extern s32 func_800A3934(void *);
extern void func_8010A8C0(void *, u8);
typedef struct { char pad[0x24]; void *vt2; char pad2[0x8C]; void *vt; } Obj;
Obj *func_8010A7D0(Obj *self, u8 k){ func_800EE3C0(self, 0x1B, k); self->vt = D_80159130; self->vt2 = D_8015C9F0; if (func_800A3934(self) == 0) func_8010A8C0(self, k); return self; }
