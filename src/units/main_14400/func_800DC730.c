#include "common.h"
extern char D_80158658[];
extern void *func_800DA8A0(void *obj, s32 kind, void *src);
typedef struct { s32 x; void *vt; } Obj;
Obj *func_800DC730(Obj *self, void *src){ func_800DA8A0(self, 0x1A, src); self->vt = D_80158658; return self; }
