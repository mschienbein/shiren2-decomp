#include "common.h"
typedef struct { char pad[0x24]; void *vtbl24; } Obj;
extern char D_8015A470[];
void func_800EFD28(Obj *, s32);
void func_800A3918(Obj *);
void func_800FC730(Obj *o, s32 flags){ o->vtbl24 = D_8015A470; func_800EFD28(o, 0); if (flags & 1) func_800A3918(o); }
