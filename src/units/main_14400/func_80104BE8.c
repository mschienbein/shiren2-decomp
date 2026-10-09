#include "common.h"
typedef struct { char pad[0x24]; void *field24; } Obj;
extern char D_8015BB38[];
extern void func_800EFD28(Obj *, s32);
extern void func_800A3918(Obj *);
void func_80104BE8(Obj *p, s32 flags) { p->field24 = D_8015BB38; func_800EFD28(p, 0); if (flags & 1) func_800A3918(p); }
