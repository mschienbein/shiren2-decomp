#include "common.h"
extern char D_80153AA0[];
extern void func_800AC68C(void *);
typedef struct { char pad[0x8]; void *vt; } Obj;
void func_80124194(Obj *self, s32 flags){ self->vt = D_80153AA0; if (flags & 1) func_800AC68C(self); }
