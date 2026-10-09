#include "common.h"
typedef struct { char pad[0x1C]; s32 *field1C; } Obj;
extern void func_800CADFC(Obj *);
extern void func_800CA0A8(s32 *, s32);
void func_800CAEB4(Obj *p) { s32 value = *p->field1C; func_800CADFC(p); func_800CA0A8(p->field1C, value); }
