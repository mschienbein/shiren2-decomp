#include "common.h"
extern char D_8015B380[];
extern void func_800EFD28(void *, s32);
extern void func_800A3918(void *);
typedef struct { char pad[0x24]; void *vt; } Obj;
void func_80100D4C(Obj *self, s32 flags){ self->vt = D_8015B380; func_800EFD28(self, 0); if (flags & 1) func_800A3918(self); }
