#include "common.h"
typedef struct { char pad[8]; void *field_8; } Obj;
extern char D_80153AA0[];
extern void func_800AC68C(Obj*);
void func_8010DE00(Obj *p,s32 flags) { p->field_8=D_80153AA0; if(flags & 1) func_800AC68C(p); }
